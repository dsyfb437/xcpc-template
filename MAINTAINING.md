# 维护说明

这仓库就一条流水线：

写 `src/` 里的板子 → 本地 `python3 print.py` 看效果 → 审完加 `@verified` → push → CI 出 PDF、发 Release。

没审完的板子也能放，只是书里不印代码。

## 目录

```
src/          板子，一个算法一个 .hpp
book/         版式（styles.typ）和代码配色（theme.tmTheme）
book.toml     书名、章节顺序
print.py      扫描 src/，生成 Typst，调 typst 出 PDF
.github/      CI
.clang-format 代码格式
```

`build/`、`output/` 是生成的，`.vscode/` 是你自己的，都不进仓库。

## 加一个板子

1. 在 `src/` 对应章里新建 `NN-英文名.hpp`；
2. 文件开头写文档块（下面讲）；
3. 代码自包含：不 include 别的板子，要用的结构直接写进来；
4. 跑 `python3 print.py --check`；
5. 审完在文档块里加 `@verified`。

文件名用 `NN-` 控制顺序，中文标题写在 `@brief` 里。章节顺序由 `book.toml` 决定，里面的 `dir` 要和 `src/` 下的目录名对上。

## 文档块

文件第一段必须是 `/** ... */`，`print.py` 靠它取标题和说明：

```cpp
/**
 * @brief 树的直径
 * @complexity O(n)
 * @index 点 1-based
 * @idea 求树上最远点对及其距离。
 * @usage auto [len, u, v] = tree_diameter(g);
 * @pitfall 边权必须非负，否则 DFS 不成立。
 * @verified
 *
 * 用途：求无权树或非负边权树的直径。
 * 做法：从任意点搜最远点，再从该点搜最远点。
 *
 * 例：
 *   auto [len, u, v] = tree_diameter(g);
 */
```

### 标签

| 标签 | 干什么 | 印在哪 |
| --- | --- | --- |
| `@brief` | 标题 | 小节标题；不写就用文件名 |
| `@complexity` | 复杂度 | 标题右边 |
| `@index` | 下标约定 | 标题右边，和复杂度用 `·` 隔开 |
| `@idea` | 一句话核心 | 字段表「核心」 |
| `@usage` | 用法 | 字段表「用法」 |
| `@pitfall` | 坑 | 字段表「坑」 |
| `@verified` | 开关：印代码 | 不显示 |
| `@print-boilerplate` | 开关：保留 `#include` / `using` | 不显示 |

`@complexity` 和 `@index` 写 `-` 等于不写。两个开关不用写值。

标签顺序随便，解析不看顺序。印出来的顺序是固定的：

1. 标题右边：复杂度 · 下标；
2. 字段表：核心 → 用法 → 坑；
3. 自由说明：`用途：`、`做法：`、`用法：`、`坑：`、`例：` 开头的行，按文件里的先后排。

### 想手动换行

在标签下面缩进两个空格以上：

```cpp
 * @usage 第一行
 *        第二行
```

这两行会算同一个 `@usage`，印出来保留换行。空行或新标签就结束续行；没缩进的行还是自由说明。

### 数学

- `@complexity` 里的 `O(...)`、`Θ(...)` 自动变成数学；
- 其它地方用 `$...$`，比如 `$x^2 >= n$`、`$O(log n)$`；
- 这是 Typst 的写法，不是 LaTeX：写 `frac(a, b)`、`sqrt(n)`，别写 `\frac`；
- 反引号里是代码，`$` 不会当公式。

### 几个坑

- 文档块必须在文件最开头；
- 文档块里不能再出现 `*/`，会提前结束注释；
- 标签拼错不会报错，只会被忽略；
- `@brief` 不写就用文件名，最好还是写。

## @verified

`@verified` 只管一件事：代码印不印。

- 没加：书里只有标题和说明；
- 加了：这一节从新页开始，带完整代码。

现在没有自动判题，全靠人看。审的时候至少确认：

- 思路和复杂度对不对；
- 下标写清楚没有；
- 空输入、边界、特殊图会不会挂；
- 复制走能不能单独编译；
- `@pitfall` 里写的限制是不是真的。

## @print-boilerplate

默认印代码时会去掉 `#include`、`#pragma once`、`using`。

如果这个板子需要整段复制，就加：

```cpp
 * @print-boilerplate
```

## 本地构建

要 Python 3 和 Typst。第一次构建会在线拉 codly，得联网。

```bash
python3 print.py            # 出 output/pdf/template.pdf
python3 print.py --watch    # 改完自动重排
python3 print.py --check    # 编译 + 行宽检查
```

出完 PDF 看一眼：章节顺序对不对、字段全不全、代码有没有超宽、加了 `@verified` 的节是不是从新页开始。

## 排版

- 每章从新页开始；
- 有 `@verified` 的节从新页开始；
- 没有的节连着排；
- 单节太长就自然续页，不裁。

字号、页边距在 `book/styles.typ` 顶部改，代码配色在 `book/theme.tmTheme`。别在板子里写排版。

## CI 和 Release

push 任何分支都会跑：装 Typst、装中文字体、`--check`、出 PDF、传 artifact。

只有 push 到 `main` 才会发 Release：

- tag：`print-<sha12>`
- 标题：`打印稿 · 年-月-日 · <sha7>`
- 附件：`template.pdf`

## 格式化

`.clang-format` 是共享规则，VS Code 的 C/C++ 扩展会读它；`.vscode/` 不进仓库。

手动格式化：

```bash
npx --yes clang-format@1.8.0 -i src/*/*.hpp
```

CI 不检查格式，别让 CI 和编辑器的 clang-format 版本互相打架。

## 来源

内容参考了 [hh2048/XCPC](https://github.com/hh2048/XCPC)（GPL-3.0）的打印稿，已按本仓库结构改写；参考稿不随仓库分发。

## 最后

一个板子一个文件，能单独复制走；没审完别加 `@verified`；改完先本地跑一遍，别让 CI 替你试错。
