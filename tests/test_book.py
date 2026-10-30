from pathlib import Path
import re
import tempfile
import unittest

from pypdf import PdfWriter
from tools.print_book import load_chapters, html_book, pdf_pages, build


class BookTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        (self.root / "src").mkdir()
        (self.root / "tools").mkdir()
        (self.root / "tools/book.css").write_text("body { color: black; }")
        (self.root / "README.md").write_text("# 测试模板册\n")

    def chapter(self, name, text):
        (self.root / "src" / name).write_text(text)

    def sample(self):
        self.chapter("01 - 基础.md", "# 基础\n\n## 演示\n\n一段说明。\n")

    @staticmethod
    def pdf(path, targets):
        writer = PdfWriter()
        for _ in range(3):
            writer.add_blank_page(595, 842)
        for target, page in targets.items():
            writer.add_named_destination(target, page)
        writer.write(path)

    def test_markdown_notes_preserve_code_and_need_no_cpp_project(self):
        self.chapter("01 - 图论.md", '# 图论\n\n## 距离\n\n先初始化数组，再运行。\n\n'
                     '```c++\nfor (int i = 0; i < n; ++i) {\n    d[i] = INF;\n}\n```\n')
        chapters = load_chapters(self.root)
        book = html_book(self.root, chapters, "version")
        self.assertIn("先初始化数组", book)
        self.assertIn("i &lt; n", book)
        self.assertIn('data-line="3"', book)
        self.assertEqual(len(chapters[0].entries), 2)

    def test_formulas_and_tables_render_without_network_scripts(self):
        self.chapter("01 - 数学.md", r'''# 数学

## 求和

复杂度为 $O(n\log n)$。

$$
\sum_{i=1}^{n}i=\frac{n(n+1)}{2}
$$

| 参数 | 含义 |
| --- | --- |
| n | 项数 |
''')
        book = html_book(self.root, load_chapters(self.root), "version")
        self.assertIn("<math", book)
        self.assertIn("<mfrac>", book)
        self.assertIn("<table>", book)
        self.assertNotIn("<script", book)

    def test_empty_chapters_are_skipped(self):
        self.sample()
        self.chapter("02 - 数学.md", "# 数学\n")
        self.assertEqual(len(load_chapters(self.root)), 1)

    def test_cpp_files_print_directly_and_leave_the_source_unchanged(self):
        path = self.root / "src/math.cpp"
        original = ('// exgcd\r\n// 求 ax + by = gcd(a, b)\r\n'
                    'auto [g, x, y] = exgcd(a, b);\r\n'
                    '    // 这行是代码里的注释\r\n').encode()
        path.write_bytes(original)
        book = html_book(self.root, load_chapters(self.root), "version")
        self.assertIn("<h2>数学</h2>", book)
        self.assertRegex(book, r'<h3[^>]+>exgcd</h3>')
        self.assertIn("求 ax + by", book)
        self.assertIn("auto [g, x, y]", book)
        self.assertIn("    // 这行是代码里的注释", book)
        self.assertEqual(path.read_bytes(), original)

    def test_new_files_in_subdirectories_need_no_registration(self):
        (self.root / "src/图论").mkdir()
        (self.root / "src/图论/最短路.cpp").write_text("dijkstra(g, s);\n")
        (self.root / "src/graph.cpp").touch()
        chapters = load_chapters(self.root)
        self.assertEqual(len(chapters), 1)
        self.assertIn("<h2>最短路</h2>", chapters[0].html)
        self.assertIn("dijkstra(g, s);", chapters[0].html)

    def test_backticks_in_cpp_strings_do_not_close_generated_code_blocks(self):
        self.chapter("literal.cpp", 'auto text = R"(```)";\n')
        chapters = load_chapters(self.root)
        self.assertIn("````cpp", chapters[0].source)
        self.assertIn("```", chapters[0].html)
        self.assertIn("</code></pre>", chapters[0].html)

    def test_nested_and_repeated_headings_get_distinct_targets(self):
        self.chapter("01 - 图论.md", "## 第三章：图论\n\n### 用法\n\n说明\n\n#### 变式\n\n说明\n")
        self.chapter("02 - 字符串.md", "# 字符串\n\n## 用法\n\n说明\n")
        chapters = load_chapters(self.root)
        targets = [target for c in chapters for target, _, _ in c.entries]
        self.assertEqual(len(targets), len(set(targets)))
        self.assertEqual(len(chapters[0].entries), 3)

    def test_unclosed_code_block_reports_filename_and_line(self):
        self.chapter("01 - 基础.md", "# 基础\n\n```cpp\nint x;\n")
        with self.assertRaisesRegex(ValueError, "01 - 基础.md 第 3 行"):
            load_chapters(self.root)

    def test_pdf_pages_are_one_based(self):
        path = self.root / "sample.pdf"
        self.pdf(path, {"/chapter-0": 1, "module-0-1": 2})
        self.assertEqual(pdf_pages(path), {"chapter-0": 2, "module-0-1": 3})

    def test_printout_uses_actual_pages_in_its_directory(self):
        self.sample()
        calls = []

        def render(source, output):
            calls.append(source.read_text())
            self.pdf(output, {"chapter-0": 1, "module-0-1": 2})

        output = build(self.root, render)
        printed = dict((target, int(page)) for target, page in
                       re.findall(r'data-target="([^"]+)">(\d+)</td>', calls[-1]))
        self.assertEqual(len(calls), 2)
        self.assertEqual(printed, pdf_pages(output))

    def test_failed_print_keeps_previous_pdf(self):
        self.sample()
        output = self.root / "output/pdf/template.pdf"
        output.parent.mkdir(parents=True)
        self.pdf(output, {"chapter-0": 1, "module-0-1": 2})
        previous = output.read_bytes()

        def render(source, output):
            self.pdf(output, {"chapter-0": 1})

        with self.assertRaisesRegex(ValueError, "目录不完整"):
            build(self.root, render)
        self.assertEqual(output.read_bytes(), previous)


if __name__ == "__main__":
    unittest.main()
