#!/usr/bin/env python3
"""把 src/ 里的板子排成 A4 打印稿。

文档块写在每个 .hpp 开头，`@verified` 控制是否印代码。
print.py 扫描 src/、按 book.toml 的章节生成 build/book.typ，再调用 Typst。

用法：
    python3 print.py            # 生成 output/pdf/template.pdf
    python3 print.py --watch    # 改完自动重排
    python3 print.py --check    # 编译 + 行宽检查
"""
from __future__ import annotations

import argparse
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass, field
import json
import os
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


CODE_WIDTH = 84

_DOC = re.compile(r"\A\ufeff?\s*/\*\*(?P<body>.*?)\*/\s*", re.S)
_TAG = re.compile(r"^@([\w-]+)\s*(.*)$")
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
    verified: bool = False
    idea: str = ""
    usage: str = ""
    pitfall: str = ""
    notes: list[str] = field(default_factory=list)
    code: list[str] = field(default_factory=list)


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
    last_tag: str | None = None
    for raw in match.group("body").splitlines():
        content = raw.strip().removeprefix("*")
        indent = len(content) - len(content.lstrip(" "))
        line = content.strip()
        tag = _TAG.match(line)
        if tag:
            key = tag.group(1)
            value = tag.group(2).strip()
            tags[key] = value if key not in tags else tags[key] + "\n" + value
            last_tag = key
        elif line and last_tag is not None and indent >= 2:
            # 缩进续行：并进上一个标签，渲染时保留换行
            tags[last_tag] += ("\n" if tags[last_tag] else "") + line
        elif line:
            notes.append(line.rstrip())
            last_tag = None
        elif notes and notes[-1] != "":
            notes.append("")
            last_tag = None

    body = text[match.end():].splitlines()


    keep = "print-boilerplate" in tags
    code = body if keep else [line for line in body if not _BOILERPLATE.match(line)]
    complexity = tags.get("complexity", "").strip()
    if complexity in {"-", "—", "–"}:
        complexity = ""
    index = tags.get("index", "").strip()
    if index in {"-", "—", "–"}:
        index = ""

    return Template(
        path=path.resolve(),
        title=tags.get("brief") or path.stem,
        complexity=complexity,
        index=index,
        verified="verified" in tags,
        idea=tags.get("idea", "").strip(),
        usage=tags.get("usage", "").strip(),
        pitfall=tags.get("pitfall", "").strip(),
        notes=trim(notes),
        code=trim(code),
    )


def load_book() -> tuple[dict, list[Chapter]]:
    meta = tomllib.loads(BOOK.read_text(encoding="utf-8"))
    chapters: list[Chapter] = []
    for entry in meta.get("chapter", []):
        directory = ALG / entry["dir"]
        templates: list[Template] = []
        if directory.is_dir():
            paths = sorted(
                directory.rglob("*"),
                key=lambda p: p.relative_to(directory).as_posix(),
            )
            for path in paths:
                if not path.is_file() or path.suffix not in {".hpp", ".cpp"}:
                    continue
                if path.name.startswith("_"):
                    continue
                template = parse_template(path)
                templates.append(template)
        chapters.append(Chapter(entry["title"], templates))
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
                    out.append(
                        f"#highlight(fill: accent-soft, stroke: 0.15pt + code-rule, radius: 1pt, extent: 1.2pt)"
                        f"[#text({json.dumps(inner, ensure_ascii=False)}, font: mono-font, size: 0.94em)]"
                    )
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


LABELS = ("用途：", "做法：", "用法：", "例：", "坑：", "注意：", "编译运行：", "管道：")


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


def field_rows(template: Template, paragraphs: list[str]) -> list[tuple[str, str, bool]]:
    rows: list[tuple[str, str, bool]] = []
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
            value = body.strip("\n").replace("`", "")
            content = f"example({json.dumps(value, ensure_ascii=False)})"
        else:
            content = f"[{inline_soft(body).replace(chr(10), ' #linebreak() ')}]"
        parts.append(f'    ("{label}", {content}),')
    return "#fields((\n" + "\n".join(parts) + "\n))"


def render(
    meta: dict,
    chapters: list[Chapter],
) -> str:
    written = [
        Chapter(chapter.title, [t for t in chapter.templates if t.verified])
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
        for template in chapter.templates:
            body: list[str] = []
            for line in template.code:
                body.extend(wrap_line(line))
            paragraphs = join_notes(template.notes)
            rows = field_rows(template, paragraphs)
            badge = mathify(template.complexity) if template.complexity else ""
            if template.index:
                badge = f"{badge} · {escape(template.index)}" if badge else escape(template.index)
            out.append(f"#cmplx[{badge}]" if badge else "#cmplx(none)")
            out.append(f"== {escape(template.title)}")
            out.append("")
            if rows:
                out.append(fields_markup(rows))
                out.append("")
            if template.code:
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


def build() -> None:
    meta, chapters = load_book()
    source = ROOT / "build" / "book.typ"
    source.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    index = {t.path: t for c in chapters for t in c.templates}
    source.write_text(render(meta, chapters), encoding="utf-8")
    subprocess.run(
        [
            find_typst(), "compile", "--root", str(ROOT),
            str(source), str(OUTPUT),
        ],
        check=True,
    )
    written_chapters = sum(1 for c in chapters if any(t.verified for t in c.templates))
    written = sum(1 for c in chapters for t in c.templates if t.verified)
    try:
        out_label = OUTPUT.relative_to(ROOT)
    except ValueError:
        out_label = OUTPUT
    print(
        f"{out_label}：{written_chapters} 章 / {written} 个已验证板子",
        flush=True,
    )


def check() -> int:
    meta, chapters = load_book()
    templates = [t for c in chapters for t in c.templates]

    def check_one(template):
        found = []
        result = subprocess.run(
            ["g++", "-std=c++20", "-fsyntax-only", "-x", "c++", str(template.path)],
            capture_output=True, text=True,
        )
        if result.returncode != 0:
            error = next((l for l in result.stderr.splitlines() if " error:" in l), "")
            found.append(f"编译失败 {template.path.relative_to(ROOT)}: {error}")
        for number, line in enumerate(template.code, 1):
            if display_width(line) > CODE_WIDTH:
                found.append(
                    f"超宽 {template.path.relative_to(ROOT)}:{number}（{display_width(line)} 列）"
                )
        return found

    problems = 0
    workers = min(8, os.cpu_count() or 4)
    with ThreadPoolExecutor(max_workers=workers) as pool:
        for found in pool.map(check_one, templates):
            for message in found:
                print(message)
                problems += 1
    if problems:
        print(f"{problems} 个问题")
        return 1
    print(f"检查通过：{len(templates)} 个板子")
    return 0


def snapshot() -> dict[Path, float]:
    paths = [BOOK, *ALG.rglob("*"), *ROOT.joinpath("book").glob("*.typ"), *ROOT.joinpath("book").glob("*.tmTheme")]
    return {p: p.stat().st_mtime for p in paths if p.is_file()}


def watch() -> None:
    print("监视 algorithms/ 和 book/，Ctrl-C 退出。", flush=True)
    build()
    last = snapshot()
    while True:
        time.sleep(0.6)
        current = snapshot()
        if current == last:
            continue
        last = current
        try:
            build()
        except subprocess.CalledProcessError:
            print("编译没过，等下一次改动。", flush=True)


def main() -> None:
    parser = argparse.ArgumentParser(description="src/ → PDF")
    parser.add_argument("--watch", action="store_true", help="改完自动重排")
    parser.add_argument("--check", action="store_true", help="只做编译和行宽检查")
    args = parser.parse_args()

    if args.check:
        raise SystemExit(check())
    if args.watch:
        try:
            watch()
        except KeyboardInterrupt:
            pass
    else:
        build()


if __name__ == "__main__":
    main()
