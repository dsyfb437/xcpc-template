#!/usr/bin/env python3
"""把 src/ 里的板子排成打印稿。

约定：
- 一个板子一个 .hpp（或 .cpp），开头是 /** ... */ 文档块；`_` 开头的文件跳过。
- `@brief` 是标题；`@complexity` 和 `@index` 一起排在标题行右端。
- `@verified` 才印代码；没标记只印文档头（审完一个加一个）。
- `@idea` / `@usage` / `@pitfall` 排进正文；其余行是自由说明。
- `#include "..."` 既是编译依赖，也是出书顺序。
- 打印时剥掉 #include / #pragma once / using，除非写了 @print-boilerplate。

用法：
    python3 print.py            # 生成 output/pdf/template.pdf
    python3 print.py --watch    # 改完自动重排
    python3 print.py --check    # 只做编译与行宽检查

换一个项目（目录里有 book.toml + src/ + book/）：
    python3 print.py --root ../my-book
    python3 print.py --config other.toml --output /tmp/book.pdf
"""
from __future__ import annotations

import argparse
from dataclasses import dataclass, field
import json
from pathlib import Path
import re
import shutil
import subprocess
import sys
import time
import tomllib
import unicodedata

ROOT = Path(__file__).resolve().parent
ALG = ROOT / "src"
BOOK = ROOT / "book.toml"
OUTPUT = ROOT / "output" / "pdf" / "template.pdf"
STYLE = ROOT / "book" / "styles.typ"


def use_project(root: Path, config: Path | None = None, output: Path | None = None) -> None:
    """切换项目根；默认值保持脚本所在目录的行为。"""
    global ROOT, ALG, BOOK, OUTPUT, STYLE
    ROOT = root.resolve()
    ALG = ROOT / "src"
    BOOK = (config if config.is_absolute() else ROOT / config).resolve() if config else ROOT / "book.toml"
    OUTPUT = (output if output.is_absolute() else ROOT / output).resolve() if output else ROOT / "output" / "pdf" / "template.pdf"
    STYLE = ROOT / "book" / "styles.typ"
    

CODE_WIDTH = 84
STATUSES = ("todo", "learning", "owned", "unreviewed")
STATUS_LABELS = {
    "todo": "待写",
    "learning": "学习中",
    "owned": "已掌握",
    "unreviewed": "未标注",
}
PRINTED_STATUSES = {"owned", "unreviewed"}

_DOC = re.compile(r"\A\ufeff?\s*/\*\*(?P<body>.*?)\*/\s*", re.S)
_TAG = re.compile(r"^@([\w-]+)\s*(.*)$")
_INCLUDE = re.compile(r'^\s*#\s*include\s+"([^"]+)"')
_INCLUDE_ANY = re.compile(r'#\s*include\s+"([^"]+)"')
_PROBLEM = re.compile(r'^\s*#define\s+PROBLEM\s+"([^"]+)"', re.M)
_BOILERPLATE = re.compile(
    r"^\s*(#\s*pragma\s+once\b.*|#\s*include\b.*|using\s+namespace\s+std\s*;"
    r"|using\s+ll\s*=\s*long\s+long\s*;)\s*$"
)
_ESCAPE = set("\\#$*_~`<@[]$")


@dataclass
class Template:
    path: Path
    title: str
    complexity: str = ""
    index: str = ""
    index_declared: bool = False
    status: str = "unreviewed"
    verified: bool = False
    idea: str = ""
    usage: str = ""
    pitfall: str = ""
    notes: list[str] = field(default_factory=list)
    code: list[str] = field(default_factory=list)
    deps: list[Path] = field(default_factory=list)


@dataclass
class Chapter:
    title: str
    templates: list[Template] = field(default_factory=list)


# ---------- 读源码 ----------

def trim(lines: list[str]) -> list[str]:
    start, end = 0, len(lines)
    while start < end and not lines[start].strip():
        start += 1
    while end > start and not lines[end - 1].strip():
        end -= 1
    return lines[start:end]


def parse_template(path: Path) -> Template:
    text = path.read_text(encoding="utf-8")
    match = _DOC.match(text)
    if not match:
        raise ValueError(f"{path.relative_to(ROOT)} 开头缺少 /** ... */ 文档块")

    tags: dict[str, str] = {}
    notes: list[str] = []
    for raw in match.group("body").splitlines():
        line = raw.strip().removeprefix("*").strip()
        tag = _TAG.match(line)
        if tag:
            tags[tag.group(1)] = tag.group(2).strip()
        elif line:
            notes.append(line.rstrip())
        elif notes and notes[-1] != "":
            notes.append("")

    body = text[match.end():].splitlines()
    deps = []
    for line in body:
        found = _INCLUDE.match(line)
        if found:
            deps.append((path.parent / found.group(1)).resolve())

    keep = "print-boilerplate" in tags
    code = body if keep else [line for line in body if not _BOILERPLATE.match(line)]
    complexity = tags.get("complexity", "").strip()
    if complexity in {"-", "—", "–"}:
        complexity = ""
    index = tags.get("index", "").strip()
    if index in {"-", "—", "–"}:
        index = ""
    status = tags.get("status", "unreviewed").strip().lower()
    if status not in STATUSES:
        raise ValueError(
            f"{path.relative_to(ROOT)} 的 @status 只能是 {'/'.join(STATUSES)}，现在是 {status!r}"
        )

    return Template(
        path=path.resolve(),
        title=tags.get("brief") or path.stem,
        complexity=complexity,
        index=index,
        index_declared="index" in tags,
        status=status,
        verified="verified" in tags,
        idea=tags.get("idea", "").strip(),
        usage=tags.get("usage", "").strip(),
        pitfall=tags.get("pitfall", "").strip(),
        notes=trim(notes),
        code=trim(code),
        deps=deps,
    )


def order(templates: list[Template], index: dict[Path, Template]) -> list[Template]:
    ordered: list[Template] = []
    state: dict[Path, int] = {}

    def visit(template: Template) -> None:
        mark = state.get(template.path, 0)
        if mark == 2:
            return
        if mark == 1:
            raise ValueError(f"循环依赖：{template.path.name}")
        state[template.path] = 1
        for dep in template.deps:
            neighbor = index.get(dep)
            if neighbor and neighbor.path.parent == template.path.parent:
                visit(neighbor)
        state[template.path] = 2
        ordered.append(template)

    for template in sorted(templates, key=lambda item: item.path.name):
        visit(template)
    return ordered


def load_book() -> tuple[dict, list[Chapter]]:
    meta = tomllib.loads(BOOK.read_text(encoding="utf-8"))
    index: dict[Path, Template] = {}
    chapters: list[Chapter] = []
    for entry in meta.get("chapter", []):
        directory = ALG / entry["dir"]
        templates: list[Template] = []
        if directory.is_dir():
            for path in sorted(directory.iterdir()):
                if path.suffix not in {".hpp", ".cpp"} or path.name.startswith("_"):
                    continue
                template = parse_template(path)
                templates.append(template)
                index[template.path] = template
        chapters.append(Chapter(entry["title"], order(templates, index)))
    return meta, chapters


# ---------- 排版 ----------

def display_width(text: str) -> int:
    return sum(2 if unicodedata.east_asian_width(ch) in "WF" else 1 for ch in text)


def cut_at(text: str, width: int) -> int:
    used = 0
    for index, ch in enumerate(text):
        used += 2 if unicodedata.east_asian_width(ch) in "WF" else 1
        if used > width:
            return index
    return len(text)


def split_comment(line: str) -> tuple[str, str]:
    quote = ""
    index = 0
    while index < len(line):
        char = line[index]
        if quote:
            if char == "\\":
                index += 2
                continue
            if char == quote:
                quote = ""
        elif char in "\"'":
            quote = char
        elif char == "/" and line[index : index + 2] == "//":
            return line[:index], line[index:]
        index += 1
    return line, ""


def space_positions(rest: str, width: int, indent: int) -> list[int]:
    positions = []
    used = 0
    for index, char in enumerate(rest):
        used += 2 if unicodedata.east_asian_width(char) in "WF" else 1
        if used > width:
            break
        if char == " " and index > indent + 6:
            positions.append(index)
    return positions


def choose_cut(rest: str, width: int, indent: int) -> int:
    positions = space_positions(rest, width, indent)
    if not positions:
        return cut_at(rest, width)

    def usable(pos: int) -> bool:
        tail = rest[pos:].strip()
        return len(tail) >= 12 and tail[:1] not in "};"

    def structural(pos: int) -> bool:
        return rest[:pos].rstrip().endswith(("&&", "||", ";", ",", "{", "}", ")"))

    good = [pos for pos in positions if usable(pos)]
    if not good:
        for pos in reversed(positions):
            if len(rest[pos:].strip()) >= 4:
                return pos + 1
        return positions[-1] + 1

    for index in range(len(good) - 1, -1, -1):
        pos = good[index]
        if structural(pos) and rest[pos + 1 :].lstrip()[:1] not in "*+-/<>=!%":
            return pos + 1
    for index in range(len(good) - 1, -1, -1):
        pos = good[index]
        if rest[pos + 1 :].lstrip()[:1] not in "*+-/<>=!%":
            return pos + 1
    return good[-1] + 1


def wrap_line(line: str, width: int = CODE_WIDTH) -> list[str]:
    if display_width(line) <= width or not line.strip():
        return [line]
    indent = len(line) - len(line.lstrip(" "))
    cont = " " * (indent + 4) + "↪ "
    code, comment = split_comment(line)
    code, comment = code.rstrip(), comment.strip()

    if comment and display_width(code + "  " + comment) <= width:
        return [code + "  " + comment]
    if code and display_width(code) <= width:
        return [code] + ([cont + comment] if comment else [])

    out = []
    rest = code or line
    while display_width(rest) > width:
        cut = choose_cut(rest, width, indent)
        out.append(rest[:cut].rstrip())
        rest = cont + rest[cut:].lstrip()
    if comment and display_width(rest + "  " + comment) <= width:
        rest = rest + "  " + comment
    elif comment:
        out.append(rest)
        rest = cont + comment
    out.append(rest)
    return out


def escape(text: str) -> str:
    out = []
    for index, ch in enumerate(text):
        if ch in _ESCAPE:
            out.append("\\" + ch)
        elif index == 0 and ch in "-+=":
            out.append("\\" + ch)
        else:
            out.append(ch)
    return "".join(out)


def inline(text: str) -> str:
    out: list[str] = []
    buffer: list[str] = []
    index = 0
    while index < len(text):
        ch = text[index]
        if ch in "$`":
            close = text.find(ch, index + 1)
            if close > index:
                if buffer:
                    out.append(escape("".join(buffer)))
                    buffer = []
                inner = text[index + 1 : close]
                out.append(f"${inner}$" if ch == "$" else f"`{inner}`")
                index = close + 1
                continue
        buffer.append(ch)
        index += 1
    if buffer:
        out.append(escape("".join(buffer)))
    return "".join(out)



def inline_soft(text: str) -> str:
    """行内代码用可断行的等宽文本，避免整段 raw 把换行顶到奇怪的位置。"""
    out: list[str] = []
    buffer: list[str] = []
    index = 0
    while index < len(text):
        ch = text[index]
        if ch in "$`":
            close = text.find(ch, index + 1)
            if close > index:
                if buffer:
                    out.append(escape("".join(buffer)))
                    buffer = []
                inner = text[index + 1 : close]
                if ch == "$":
                    out.append(f"${inner}$")
                else:
                    out.append(f"#text(font: mono-font, size: 0.94em)[{escape(inner)}]")
                index = close + 1
                continue
        buffer.append(ch)
        index += 1
    if buffer:
        out.append(escape("".join(buffer)))
    return "".join(out)


def mathify(text: str) -> str:
    out: list[str] = []
    buffer: list[str] = []
    index = 0
    while index < len(text):
        char = text[index]
        if char in "OΘ" and index + 1 < len(text) and text[index + 1] == "(":
            depth = 0
            close = index + 1
            while close < len(text):
                if text[close] == "(":
                    depth += 1
                elif text[close] == ")":
                    depth -= 1
                    if depth == 0:
                        break
                close += 1
            if close < len(text):
                if buffer:
                    out.append(escape("".join(buffer)))
                    buffer = []
                inner = re.sub(r"\bsqrt\s+([A-Za-z0-9]+)", r"sqrt(\1)", text[index + 2 : close])
                out.append(f"${char}({inner})$")
                index = close + 1
                continue
        buffer.append(char)
        index += 1
    if buffer:
        out.append(escape("".join(buffer)))
    return "".join(out)


LABELS = ("用途：", "做法：", "用法：", "例：", "坑：", "依赖：", "注意：", "编译运行：", "管道：")


def starts_section(line: str) -> bool:
    return line.strip().startswith(LABELS)


def looks_code(line: str) -> bool:
    s = line.strip()
    return s.startswith(("#", "//")) or any(ch in s for ch in ";{}") or s.endswith(";")


def join_notes(lines: list[str]) -> list[str]:
    paragraphs: list[list[str]] = [[]]
    for line in lines:
        if line.strip():
            paragraphs[-1].append(line.strip())
        elif paragraphs[-1]:
            paragraphs.append([])
    merged = []
    for chunk in paragraphs:
        text = ""
        prev = ""
        for piece in chunk:
            if not text:
                text = piece
            elif looks_code(prev) or looks_code(piece) or starts_section(piece):
                merged.append(text)
                text = piece
            elif text[-1].isascii() and text[-1].isalnum() and piece[0].isascii() and piece[0].isalnum():
                text += " " + piece
            else:
                text += piece
            prev = piece
        if text:
            merged.append(text)
    return merged


def split_note(paragraph: str) -> tuple[str, str]:
    for label in LABELS:
        if paragraph.startswith(label):
            return label[:-1], paragraph[len(label):].strip()
    return "", paragraph


def codeish(text: str) -> bool:
    s = text.strip()
    return any(ch in s for ch in ";{}") or s.startswith(("#", "//", "auto ", "if ", "for "))


def field_rows(
    template: Template, deps: list[str], paragraphs: list[str]
) -> list[tuple[str, str, bool]]:
    rows: list[tuple[str, str, bool]] = []
    if deps:
        rows.append(("依赖", "、".join(deps), False))
    if template.idea:
        rows.append(("核心", template.idea, False))
    if template.usage:
        rows.append(("用法", template.usage, False))
    if template.pitfall:
        rows.append(("坑", template.pitfall, False))
    for paragraph in paragraphs:
        label, body = split_note(paragraph)
        if label:
            rows.append((label, body, label == "例" and codeish(body)))
        elif rows:
            prev_label, prev_body, prev_example = rows[-1]
            rows[-1] = (
                prev_label,
                prev_body + "\n" + paragraph,
                prev_example or codeish(paragraph),
            )
        else:
            rows.append(("说明", paragraph, False))
    return rows


def fields_markup(rows: list[tuple[str, str, bool]]) -> str:
    parts = []
    for label, body, is_example in rows:
        if is_example:
            # 例子按字符串交给 Typst：里面的 // 和引号不能当标记解析。
            value = body.replace("`", "")
            content = f"example({json.dumps(value, ensure_ascii=False)})"
        else:
            content = f"[{inline_soft(body).replace(chr(10), ' ')}]"
        parts.append(f'    ("{label}", {content}),')
    return "#fields((\n" + "\n".join(parts) + "\n))"


def render(
    meta: dict,
    chapters: list[Chapter],
    index: dict[Path, Template],
    include_all: bool = False,
) -> str:
    def shown(template: Template) -> bool:
        return include_all or template.status in PRINTED_STATUSES

    written = [
        Chapter(chapter.title, [t for t in chapter.templates if shown(t)])
        for chapter in chapters
    ]
    written = [chapter for chapter in written if chapter.templates]

    out = ['#import "/book/styles.typ": *', "#show: book.with("]
    for key in ("title", "name", "short", "subtitle", "edition", "author"):
        out.append(f'  {key}: "{meta.get(key, "")}",')
    out.append("  chapters: (")
    for chapter in written:
        out.append(f'    "{escape(chapter.title)}",')
    out.append("  ),")
    out.append(f'  count: {sum(len(c.templates) for c in written)},')
    out.append(f'  duplex: {str(bool(meta.get("duplex", False))).lower()},')
    out.append(")")
    out.append("")

    for chapter in written:
        out.append("#pagebreak(weak: true)")
        out.append(f"= {escape(chapter.title)}")
        out.append("")
        for position, template in enumerate(chapter.templates):
            body: list[str] = []
            for line in template.code:
                body.extend(wrap_line(line))
            paragraphs = join_notes(template.notes)
            deps = [
                index[dep].title
                for dep in template.deps
                if dep in index and shown(index[dep])
            ]
            rows = field_rows(template, deps, paragraphs)
            # 每节从新页开始：标题一定在页首，超长自然续页，不做高度估算
            if position > 0 and template.verified:
                out.append("#pagebreak(weak: true)")
            badge = mathify(template.complexity) if template.complexity else ""
            if template.index:
                badge = f"{badge} · {escape(template.index)}" if badge else escape(template.index)
            out.append(f"#cmplx[{badge}]" if badge else "#cmplx(none)")
            out.append(f"== {escape(template.title)}")
            out.append("")
            if rows:
                out.append(fields_markup(rows))
                out.append("")
            if template.code and template.verified:
                out.append("#code[")
                out.append("```cpp")
                out.extend(body)
                out.append("```")
                out.append("]")
                out.append("")
    return "\n".join(out).rstrip() + "\n"


# ---------- 构建 ----------

def find_typst() -> str:
    found = shutil.which("typst")
    local = Path.home() / ".local" / "bin" / "typst"
    if found:
        return found
    if local.exists():
        return str(local)
    sys.exit("找不到 typst：装一个（单文件二进制），或看 README。")


def tested_boards() -> set[Path]:
    found: set[Path] = set()
    directory = ROOT / "verify"
    if not directory.is_dir():
        return found
    for path in directory.rglob("*.test.cpp"):
        text = path.read_text(encoding="utf-8")
        problem = _PROBLEM.search(text)
        if not problem or problem.group(1).strip() == "TODO":
            continue
        for include in _INCLUDE_ANY.findall(text):
            target = (path.parent / include).resolve()
            if target.is_file():
                found.add(target)
    return found


def pad(text: str, width: int) -> str:
    return text + " " * max(0, width - display_width(text))


def status_report() -> int:
    meta, chapters = load_book()
    tested = tested_boards()
    print(f"{pad('章', 16)}{pad('板子', 32)}{pad('状态', 8)}{pad('测试', 6)}行数")
    print("-" * 72)
    counts = {status: 0 for status in STATUSES}
    total = 0
    empty: list[str] = []
    no_index: list[str] = []
    for chapter in chapters:
        if not chapter.templates:
            empty.append(chapter.title)
            continue
        for template in chapter.templates:
            counts[template.status] += 1
            total += 1
            relative = template.path.relative_to(ROOT)
            name = relative.parts[1] if len(relative.parts) > 2 else relative.parts[0]
            test = "✓" if template.path in tested else "-"
            print(
                f"{pad(name, 16)}{pad(template.title, 32)}"
                f"{pad(STATUS_LABELS[template.status], 8)}{pad(test, 6)}{len(template.code)}"
            )
            if not template.index_declared:
                no_index.append(template.title)
    print("-" * 72)
    summary = "、".join(f"{STATUS_LABELS[status]} {counts[status]}" for status in STATUSES)
    print(f"合计 {total} 个板子：{summary}")
    print(f"有 LC 测试：{sum(1 for c in chapters for t in c.templates if t.path in tested)} 个")
    if empty:
        print("空章（book.toml 里有、alg/ 里没有板子）：" + "、".join(empty))
    if no_index:
        print(f"缺 @index：{len(no_index)} 个（{'、'.join(no_index[:8])}{'…' if len(no_index) > 8 else ''}）")
    print("提示：默认只排「已掌握 + 未标注」；--all 把 todo/learning 也排出来复习。")
    return 0


def build(include_all: bool = False) -> None:
    meta, chapters = load_book()
    source = ROOT / "build" / "book.typ"
    source.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    index = {t.path: t for c in chapters for t in c.templates}
    source.write_text(render(meta, chapters, index, include_all), encoding="utf-8")
    subprocess.run(
        [
            find_typst(), "compile", "--root", str(ROOT),
            str(source), str(OUTPUT),
        ],
        check=True,
    )
    written_chapters = sum(
        1 for c in chapters if any(include_all or t.status in PRINTED_STATUSES for t in c.templates)
    )
    written = sum(1 for c in chapters for t in c.templates if include_all or t.status in PRINTED_STATUSES)
    owned = sum(1 for c in chapters for t in c.templates if t.status == "owned")
    try:
        out_label = OUTPUT.relative_to(ROOT)
    except ValueError:
        out_label = OUTPUT
    print(
        f"{out_label}：{written_chapters} 章 / {written} 个板子",
        flush=True,
    )


def check() -> int:
    meta, chapters = load_book()
    problems = 0
    for chapter in chapters:
        for template in chapter.templates:
            result = subprocess.run(
                ["g++", "-std=c++20", "-fsyntax-only", "-x", "c++", str(template.path)],
                capture_output=True, text=True,
            )
            if result.returncode != 0:
                error = next((l for l in result.stderr.splitlines() if " error:" in l), "")
                print(f"编译失败 {template.path.relative_to(ROOT)}: {error}")
                problems += 1
            for number, line in enumerate(template.code, 1):
                if display_width(line) > CODE_WIDTH:
                    print(f"超宽 {template.path.relative_to(ROOT)}:{number}（{display_width(line)} 列）")
                    problems += 1
    if problems:
        print(f"{problems} 个问题")
        return 1
    print(f"检查通过：{sum(len(c.templates) for c in chapters)} 个板子")
    return 0


def snapshot() -> dict[Path, float]:
    paths = [BOOK, *ALG.rglob("*"), *ROOT.joinpath("book").glob("*.typ"), *ROOT.joinpath("book").glob("*.tmTheme")]
    return {p: p.stat().st_mtime for p in paths if p.is_file()}


def watch(include_all: bool = False) -> None:
    print("监视 algorithms/ 和 book/，Ctrl-C 退出。", flush=True)
    build(include_all)
    last = snapshot()
    while True:
        time.sleep(0.6)
        current = snapshot()
        if current == last:
            continue
        last = current
        try:
            build(include_all)
        except subprocess.CalledProcessError:
            print("编译没过，等下一次改动。", flush=True)


def main() -> None:
    parser = argparse.ArgumentParser(description="alg/ → PDF")
    parser.add_argument("--watch", action="store_true", help="改完自动重排")
    parser.add_argument("--check", action="store_true", help="只做编译和行宽检查")
    parser.add_argument("--all", action="store_true", help="连 todo/learning 一起排（复习用）")
    parser.add_argument("--root", help="项目根目录（默认：脚本所在目录）")
    parser.add_argument("--config", help="book.toml 路径（相对 --root）")
    parser.add_argument("--output", help="输出 PDF 路径（相对 --root）")
    args = parser.parse_args()

    root = Path(args.root) if args.root else Path(__file__).resolve().parent
    config = Path(args.config) if args.config else None
    output = Path(args.output) if args.output else None
    use_project(root, config, output)

    if args.check:
        raise SystemExit(check())
    if args.watch:
        try:
            watch(args.all)
        except KeyboardInterrupt:
            pass
    else:
        build(args.all)


if __name__ == "__main__":
    main()
