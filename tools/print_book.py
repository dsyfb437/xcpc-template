"""读取 src/ 里的代码和笔记，生成带目录页码的打印稿。"""
from dataclasses import dataclass
from html import escape
from pathlib import Path
import re
import shutil
import subprocess
import sys
import tempfile

import mistune
from latex2mathml.converter import convert
from pypdf import PdfReader

ROOT = Path(__file__).resolve().parent.parent
TITLES = {
    "main": "主函数",
    "ds": "数据结构",
    "why_you_cannot_write_these": "并查集",
    "graph": "图论",
    "math": "数学",
    "string": "字符串",
    "cg": "凸包与最大三角形",
    "cg_": "几何",
}


class Renderer(mistune.HTMLRenderer):
    def __init__(self, chapter, base):
        super().__init__(escape=True)
        self.chapter, self.base, self.entries = chapter, base, []
        self.sequence = 0

    def heading(self, text, level, **attrs):
        self.sequence += 1
        target = f"module-{self.chapter}-{self.sequence}"
        level = min(6, max(3, level - self.base + 2))
        if level <= 4:
            self.entries.append((target, text, "module" if level == 3 else "subsection"))
        return f'<h{level} id="{target}">{text}</h{level}>\n'

    def block_code(self, code, info=None):
        lines = [f'<span class="code-line" data-line="{n}">{escape(line) or " "}</span>'
                 for n, line in enumerate(code.rstrip("\n").split("\n"), 1)]
        if len(lines) <= 8:
            content = '<span class="code-keep">' + "".join(lines) + '</span>'
        else:
            content = ('<span class="code-keep">' + "".join(lines[:4]) + '</span>'
                       + "".join(lines[4:-4])
                       + '<span class="code-keep">' + "".join(lines[-4:]) + '</span>')
        return f"<pre><code>{content}</code></pre>\n"

    def inline_math(self, text):
        return convert(text, display="inline")

    def block_math(self, text):
        return '<div class="equation">' + convert(text, display="block") + '</div>\n'

    def block_html(self, html):
        return "" if html.strip().startswith("<!--") else super().block_html(html)


@dataclass
class Chapter:
    source: str
    html: str
    entries: list


def check_fences(source, name):
    opened = None
    for number, line in enumerate(source.splitlines(), 1):
        match = re.match(r"^ {0,3}(`{3,}|~{3,})(.*)$", line)
        if not match:
            continue
        marker, suffix = match.groups()
        if opened is None:
            opened = marker, number
        elif marker[0] == opened[0][0] and len(marker) >= len(opened[0]) and not suffix.strip():
            opened = None
    if opened:
        raise ValueError(f"{name} 第 {opened[1]} 行的代码块没有结束标记。")


def cpp_markdown(source):
    """顶格的连续 // 注释用作标题和说明，代码仍只在源文件里维护。"""
    parts, code = [], []

    def flush():
        if any(line.strip() for line in code):
            text = "".join(code).rstrip("\n")
            longest = max((len(run) for run in re.findall(r"`+", text)), default=0)
            fence = "`" * max(3, longest + 1)
            parts.append(f"{fence}cpp\n{text}\n{fence}\n")
        code.clear()

    lines = iter(source.splitlines(keepends=True))
    comments = []
    for line in lines:
        if line.startswith("//"):
            flush()
            comments.append(line[2:].strip())
        else:
            if comments:
                heading, *notes = comments
                parts.append("## " + heading.strip("= ") + "\n")
                parts.extend(note + "\n" for note in notes if note)
                comments.clear()
            code.append(line)
    if comments:
        heading, *notes = comments
        parts.append("## " + heading.strip("= ") + "\n")
        parts.extend(note + "\n" for note in notes if note)
    flush()
    return "\n".join(parts)


def load_chapters(root):
    chapters = []
    order = {name: index for index, name in enumerate(TITLES)}
    paths = sorted((path for path in (root / "src").rglob("*")
                    if path.suffix in {".md", ".cpp", ".hpp", ".cc", ".cxx"}),
                   key=lambda path: (order.get(path.stem, len(order)), str(path)))
    for index, path in enumerate(paths):
        source = path.read_text(encoding="utf-8-sig").strip()
        if not source:
            continue
        if path.suffix != ".md":
            title = TITLES.get(path.stem, path.stem)
            source = f"# {title}\n\n" + cpp_markdown(source)
        check_fences(source, path.name)
        title = re.sub(r"^\d+[A-Z]?\s*[-_]\s*", "", path.stem)
        base, body = 1, source
        first, _, rest = source.partition("\n")
        match = re.fullmatch(r"(#{1,2})\s+(.+)", first)
        if match:
            base, title, body = len(match[1]), match[2].strip(), rest.strip()
        if not re.sub(r"<!--.*?-->", "", body, flags=re.S).strip():
            continue
        renderer = Renderer(index, base)
        markdown = mistune.create_markdown(renderer=renderer, plugins=["table", "math", "strikethrough"])
        try:
            content = markdown(body)
        except Exception as error:
            raise ValueError(f"{path.name}：{error}") from error
        target = f"chapter-{index}"
        html = f'<section class="chapter" id="{target}"><h2>{escape(title)}</h2>\n{content}</section>'
        chapters.append(Chapter(source, html, [(target, escape(title), "chapter"), *renderer.entries]))
    return chapters


def html_book(root, chapters, revision, pages=None):
    pages = pages or {}
    css = (root / "tools/book.css").read_text(encoding="utf-8")
    title = (root / "README.md").read_text(encoding="utf-8").splitlines()[0].lstrip("# ")
    rows = []
    for chapter in chapters:
        for target, label, kind in chapter.entries:
            rows.append(f'<tr class="toc-{kind}"><td><a href="#{target}">{label}</a></td>'
                        f'<td data-target="{target}">{pages.get(target, "-")}</td></tr>')
    return (f'<!doctype html><html lang="zh-CN"><meta charset="utf-8"><title>{escape(title)}</title>'
            f'<style>{css}</style><body><header><p class="eyebrow">XCPC TEMPLATE</p>'
            f'<h1>{escape(title)}</h1><p class="lead">线下比赛打印稿</p>'
            f'<p class="edition">源码版本：{escape(revision)}</p></header>'
            '<h2 class="toc-title">目录</h2><table class="toc"><thead><tr><th>内容</th><th>页码</th>'
            '</tr></thead><tbody>' + "".join(rows) + '</tbody></table>'
            + "".join(chapter.html for chapter in chapters) + '</body></html>')


def pdf_pages(path):
    reader = PdfReader(path)
    pages = {}
    for key, destination in reader.named_destinations.items():
        target = key.lstrip("/")
        if target.startswith(("chapter-", "module-")):
            page = reader.get_destination_page_number(destination)
            if page is None or page < 0:
                raise ValueError(f"目录页码缺失：{target}")
            pages[target] = page + 1
    return pages


def print_pdf(html, output, chrome):
    with tempfile.TemporaryDirectory(prefix="xcpc-print-browser-") as directory:
        temporary = Path(directory) / "print.pdf"
        result = subprocess.run([chrome, "--headless", "--disable-gpu", "--no-pdf-header-footer",
                                 f"--user-data-dir={directory}/profile", f"--print-to-pdf={temporary}",
                                 html.resolve().as_uri()], capture_output=True, text=True, timeout=90)
        if result.returncode or not temporary.is_file():
            raise ValueError("打印失败：" + result.stderr[-1500:])
        shutil.copyfile(temporary, output)


def source_revision(root):
    try:
        result = subprocess.run(["git", "rev-parse", "--short", "HEAD"], cwd=root,
                                capture_output=True, text=True, timeout=5)
        if result.returncode:
            return "本地打印"
        dirty = subprocess.run(["git", "status", "--porcelain"], cwd=root,
                               capture_output=True, text=True, timeout=5).stdout.strip()
        return result.stdout.strip() + ("（含本地修改）" if dirty else "")
    except (OSError, subprocess.TimeoutExpired):
        return "本地打印"


def build(root=ROOT, render=None):
    chapters = load_chapters(root)
    output = root / "output"
    output.mkdir(exist_ok=True)
    html = output / "template.html"
    pdf = output / "pdf/template.pdf"
    (output / "template.md").write_text("\n\n".join(c.source for c in chapters) + "\n", encoding="utf-8")
    if render is None:
        chrome = next((shutil.which(name) for name in ("google-chrome", "chromium", "chromium-browser")
                       if shutil.which(name)), None)
        if not chrome:
            raise ValueError("本地打印需要 Chrome / Chromium；也可以直接下载仓库里的打印稿。")
        render = lambda source, target: print_pdf(source, target, chrome)
    pages = {}
    expected = {target for chapter in chapters for target, _, _ in chapter.entries}
    revision = source_revision(root)
    with tempfile.TemporaryDirectory(prefix="xcpc-print-pages-") as directory:
        temporary = Path(directory) / "print.pdf"
        for _ in range(3):
            html.write_text(html_book(root, chapters, revision, pages), encoding="utf-8")
            render(html, temporary)
            actual = pdf_pages(temporary)
            if set(actual) != expected:
                raise ValueError("目录不完整，保留上一份打印稿。")
            if actual == pages:
                pdf.parent.mkdir(exist_ok=True)
                shutil.copyfile(temporary, pdf)
                return pdf
            pages = actual
    raise ValueError("目录页码没有稳定，保留上一份打印稿。")


def main():
    try:
        print(build())
    except Exception as error:
        sys.exit(f"生成失败：{error}")
