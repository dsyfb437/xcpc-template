# 维护说明

加板子就在 `src/` 对应章里新建 `NN-名字.hpp`，`NN` 控制章内顺序。文件名用英文，中文标题写在文件开头的注释里。

每个文件开头要有一段 `/** ... */`，`print.py` 从里面取标题和说明。常用的标签：

- `@brief`：标题，不写就用文件名
- `@complexity`：复杂度，印在标题右边
- `@index`：下标约定，和复杂度印在一起
- `@idea`、`@usage`、`@pitfall`：分别印成核心、用法、坑
- `@verified`：加了才印代码
- `@print-boilerplate`：保留 `#include`、`using`

标签顺序无所谓。印出来的顺序是固定的：标题右边先复杂度再下标，下面依次是核心、用法、坑，最后是自由说明。自由说明里以 `用途：`、`做法：`、`例：` 开头的行会单独成行。

一个标签想写多行，就在下面缩进两个空格，中间别空行：

```cpp
 * @usage 第一行
 *        第二行
```

公式用 `$...$`，是 Typst 语法，不是 LaTeX，写 `frac(a, b)` 而不是 `\frac`。`@complexity` 里的 `O(...)` 会自动变成公式。

代码默认不印。审完在注释里加一行 `@verified`，这一节才会带代码印出来，并且从新页开始。没审完的就别加。

每个文件要能单独复制走，不要 include 仓库里的其他文件，需要的小结构直接写进来。

本地构建用 `python3 print.py`，`--watch` 是改完自动重排，`--check` 只检查编译和行宽。第一次构建会在线下载 codly，需要联网。

push 之后 CI 会装 Typst、跑检查、生成 PDF。只有 `main` 会发 Release，tag 是 `print-<sha12>`，标题是日期加短 sha。

版式在 `book/styles.typ`，章节顺序在 `book.toml`。格式化用 `.clang-format`，VS Code 的 C/C++ 扩展会读它；`.vscode/` 不进仓库。

内容参考了 hh2048/XCPC（GPL-3.0）的打印稿，结构和排版已经改过。
