# 维护文档

这份文档覆盖仓库的结构、约定、文档块语法、构建、排版、CI 和常见故障。
改仓库之前先看这一份就够了。

## 1. 项目概览

这是一个打印优先的 XCPC 模板册。每个算法一个 `.hpp` 文件，文件开头的注释块
提供标题和说明，`print.py` 把这些文件排成 Typst 文档，最后编译成 A4 PDF。

流程：

```
src/ 里的 .hpp
      │  print.py 扫描、解析文档块
      ▼
build/book.typ
      │  typst compile
      ▼
output/pdf/template.pdf
```

同一套流程在 GitHub Actions 上跑，`main` 分支构建成功后会发布 Release。

技术栈：

- Python 3（只用标准库）；
- Typst 0.15.1，负责排版和 PDF；
- codly 1.3.0，Typst 包，负责代码行号和语法高亮；
- Noto CJK 字体，CI 里安装，本地需自行安装。

## 2. 目录结构

```
src/                       算法模板，一个算法一个 .hpp
  01-basics/               基础
  02-trees/                树上问题
  03-graphs/               图论
  04-network-flow/         网络流
  05-number-theory/        数论
  06-geometry/             几何
  08-data-structures/      数据结构
  09-dynamic-programming/  动态规划
  10-strings/              字符串
  14-misc/                 杂项

book/
  styles.typ               页面、字体、字段、代码块的样式
  theme.tmTheme            代码语法配色

book.toml                  书名、章节顺序
print.py                   扫描 → 生成 Typst → 调 typst 编译
.github/workflows/check.yml  CI：检查、构建、发布
.clang-format              代码格式规则
.gitignore                 忽略生成目录和个人配置
README.md                  使用说明
MAINTAINING.md             本文档
```

生成目录：

- `build/`：`print.py` 生成的 `book.typ`；
- `output/`：PDF 和预览图。

这两个目录已在 `.gitignore` 中，不要提交。`.vscode/` 是个人编辑器配置，
同样被忽略。

## 3. 环境与快速开始

本地需要：

- Python 3.12 或更新；
- Typst 0.15.x，放在 `PATH` 或 `~/.local/bin/typst`；
- 中文字体（Noto Sans CJK、Noto Sans Mono CJK）；
- `g++`，`--check` 编译模板时要用。

第一次构建时 Typst 会在线下载 `@preview/codly:1.3.0`，需要网络；之后走缓存。

```bash
python3 print.py            # 生成 output/pdf/template.pdf
python3 print.py --watch    # 监听改动，自动重排
python3 print.py --check    # 只做编译和行宽检查
```

`--check` 会用 `g++ -std=c++20 -fsyntax-only` 逐个编译模板，并检查每行是否
超过 84 列；不生成 PDF，适合提交前跑。

## 4. 文件与章节约定

- 目录名：小写英文，两位章节号加短名，例如 `05-number-theory/`；
- 文件名：`NN-英文短名.hpp`，例如 `05-centroid-decomposition.hpp`；
- `NN-` 只用于控制同一章内的打印顺序，不表达算法含义；
- 一个文件只放一个算法，文件名和内容要对应；
- 中文标题写在文档块的 `@brief` 里，文件名不用中文，避免跨平台编码问题；
- 章节的目录名、显示名和顺序由 `book.toml` 决定，`dir` 必须和 `src/` 下的
  目录名一致。

`book.toml` 示例：

```toml
[[chapter]]
dir = "02-trees"
title = "树上问题"
```

没有模板的章节可以保留在 `book.toml` 里，构建时会跳过。

## 5. 自包含原则

每个模板都必须能单独复制走：

- 不 `#include` 仓库里的其他模板；
- 需要的小结构、辅助函数、类型别名直接写在当前文件里；
- 不建立 `utils.hpp`、`common.hpp` 这类公共依赖；
- 复制一个 `.hpp` 到比赛代码里就能编译。

允许少量重复。为了少写几行而引入跨文件依赖，赛场上容易漏带文件。

## 6. 文档块

每个 `.hpp` 文件的第一段必须是 `/** ... */` 文档块，`print.py` 从这里提取
标题、复杂度和说明。文档块之前不能有其他代码或注释。

### 6.1 完整示例

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

### 6.2 标签

| 标签 | 作用 | 打印位置 |
| --- | --- | --- |
| `@brief` | 小节标题 | 标题；不写则用文件名 |
| `@complexity` | 时间复杂度 | 标题右端 |
| `@index` | 下标约定 | 标题右端，和复杂度用 `·` 分隔 |
| `@idea` | 一句话核心 | 字段表「核心」 |
| `@usage` | 使用方式 | 字段表「用法」 |
| `@pitfall` | 限制、坑 | 字段表「坑」 |
| `@verified` | 是否打印代码 | 不显示 |
| `@print-boilerplate` | 是否保留 `#include` / `using` | 不显示 |

补充说明：

- `@complexity` 和 `@index` 写 `-` 等同于不写，可以直接省略；
- `@verified`、`@print-boilerplate` 是开关，后面不写值；
- 同一个标签写多次时，会按换行拼接到一起；
- 未识别的标签会被静默忽略，不会报错。

### 6.3 解析顺序与显示顺序

标签在文件里的先后不影响解析结果，`print.py` 按键名取值。

打印时的顺序是固定的：

1. 标题右端：复杂度 · 下标；
2. 字段表：核心 → 用法 → 坑；
3. 自由说明：按源文件中的出现顺序。

建议的书写顺序是：

```cpp
@brief → @complexity → @index → @idea → @usage → @pitfall
→ @verified / @print-boilerplate → 自由说明
```

### 6.4 手动换行

一个标签的内容需要写成多行时，在标签下面缩进两个空格以上：

```cpp
 * @usage 第一行
 *        第二行
```

规则：

- 续行必须紧跟在标签下面，中间不能空行；
- 缩进至少两个空格；
- 续行会并入上一个标签，打印时保留换行；
- 空行或新的 `@标签` 会结束续行；
- 没有缩进的行仍然按自由说明处理。

### 6.5 自由说明

标签之外的内容是自由说明。以这些前缀开头的行会单独成行：

- `用途：`
- `做法：`
- `用法：`
- `坑：`
- `例：`

其中 `例：` 在内容看起来像代码时会用等宽字体和左侧竖线，适合放调用示例；
纯文字仍然按普通字段排版。其他自由段落
按出现顺序保留，空行用于分段。

`用途：`、`做法：` 这类说明适合放段落文字；`例：` 适合放代码。不要把
大段代码塞进 `@pitfall`，那样不会按代码块排版。

### 6.6 数学

`@complexity` 里的 `O(...)` 和 `Θ(...)` 会自动转成 Typst 数学：

```cpp
@complexity O(n log n)
@complexity 预处理 O(n)，单次 O(log n)
```

第二行只把两个 `O(...)` 包成数学，中文保持正文。

其他位置用 `$...$` 写数学：

```cpp
@pitfall 需要满足 $x^2 >= n$，复杂度 $O(log n)$。
```

注意这是 Typst 语法，不是 LaTeX：

| 想要 | 写法 |
| --- | --- |
| 上标 | `$x^2$` |
| 下标 | `$a_i$` |
| 大于等于 | `$x >= n$` |
| 分数 | `$frac(a, b)$` |
| 根号 | `$sqrt(n)$` |
| 求和 | `$sum_(i=1)^n i$` |
| 希腊字母 | `$alpha$`、`$Theta$` |

反引号里的内容是代码，`$` 不会按数学解析。落单的 `$` 会被转义成普通字符。

### 6.7 行内代码

用反引号包起来的内容按行内代码排版：等宽字体、浅色底、细边框，跨行时
底色跟随断行。

```cpp
@usage 例：`first_true(0, 1e9, [&](ll x) { return x * x >= n; })`
```

行内代码里的 `//`、引号、方括号都会按普通字符处理，不会破坏 Typst 标记。

### 6.8 常见错误

- 文档块里出现 `*/`：会提前结束注释，后面的内容被当成代码；
- 标签拼错：不会报错，字段直接丢失；
- 文档块不在文件开头：解析失败，构建报错；
- 在标签值里写 LaTeX：`\frac`、`\log` 在 Typst 里不成立；
- 超过 84 列：`--check` 会报，打印时也会折行。

## 7. @verified 与审阅

`@verified` 只控制代码是否进入 PDF：

| 状态 | 打印说明 | 打印代码 | 分页 |
| --- | --- | --- | --- |
| 没有 `@verified` | 是 | 否 | 连续排 |
| 有 `@verified` | 是 | 是 | 从新页开始 |

未验证的模板可以提交，书里只显示标题和说明，不会留出大段空白。

审阅时至少检查：

- 算法思路和复杂度是否正确；
- 下标约定是否写清楚；
- 空输入、边界情况、特殊图是否处理；
- 是否自包含，单独复制能否编译；
- `@pitfall` 里写的限制是否真的成立；
- 代码行宽是否超过 84 列。

确认后在文档块里加一行：

```cpp
 * @verified
```

## 8. @print-boilerplate

默认打印时会剥掉以下几类前置行：

- `#include ...`
- `#pragma once`
- `using namespace std;`
- `using ll = long long;`

如果这个模板需要整段复制走，在文档块里加：

```cpp
 * @print-boilerplate
```

加上后，上述行会原样出现在打印稿里。

## 9. 构建与检查

### 9.1 命令

```bash
python3 print.py            # 构建 PDF
python3 print.py --watch    # 监听改动自动重排
python3 print.py --check    # 编译 + 行宽检查
```

构建路径固定：

- 生成 Typst：`build/book.typ`
- 输出 PDF：`output/pdf/template.pdf`

`print.py` 不接受自定义路径参数，换了仓库位置也能直接跑。

### 9.2 构建后检查

- 章节顺序和 `book.toml` 一致；
- 每章从新页开始；
- 加了 `@verified` 的节从新页开始，带完整代码；
- 未验证的节连续排列，没有异常空白；
- 代码没有超宽或折行错位；
- 标题右端的复杂度和下标显示正常。

### 9.3 行宽

`CODE_WIDTH = 84`。超过 84 列的代码行会在 `--check` 中报出，打印时会被
`wrap_line` 折行，折行处可能影响阅读。新增代码尽量控制在 84 列内。

## 10. 排版

### 10.1 分页规则

- 每章从新页开始；
- 有 `@verified` 的节从新页开始；
- 没有 `@verified` 的节连续排；
- 单节超过一页时自然续页，不裁切、不强制缩放。

### 10.2 样式参数

常用参数在 `book/styles.typ` 顶部：

- `body-size`：正文字号；
- `field-size`：字段表字号；
- `code-size`：代码字号；
- 页边距、字体在文件开头的 `#set page` 和字体变量里。

代码配色在 `book/theme.tmTheme`，和页面布局独立。

不要在算法模板里写排版相关的设置。

### 10.3 章节

章节顺序由 `book.toml` 的 `[[chapter]]` 顺序决定。新增章节时同步检查：

1. `src/` 下有对应目录；
2. `book.toml` 里有对应条目；
3. 目录名和 `dir` 完全一致。

## 11. CI 与发布

工作流文件：`.github/workflows/check.yml`。

触发条件：

- push（任意分支）；
- pull request；
- 手动 `workflow_dispatch`。

构建任务：

1. checkout；
2. 安装 Python；
3. 下载并安装 Typst 0.15.1；
4. 安装 Noto CJK 字体；
5. `python3 print.py --check`；
6. `python3 print.py`；
7. 上传 `template.pdf` artifact（保留 30 天）。

发布任务只在 `main` 分支运行：

- tag：`print-<sha12>`；
- 标题：`打印稿 · YYYY-MM-DD · <sha7>`；
- 附件：`template.pdf`；
- 发布前会检查 `main` 最新 commit 是否就是本次构建，避免旧构建覆盖新版本。

其他分支只构建，不发布。

## 12. 格式化

`.clang-format` 是共享规则，主要内容：

- LLVM 风格；
- 4 空格缩进；
- 84 列；
- K&R 大括号；
- 短函数允许单行；
- 注释不重排；
- 空行最多保留 1 行。

VS Code 的 C/C++ 扩展会读取它；`.vscode/settings.json` 已配置保存时格式化，
但 `.vscode/` 被 gitignore，不会进仓库。

手动格式化：

```bash
npx --yes clang-format@1.8.0 -i src/*/*.hpp
```

CI 不检查格式，避免 CI 和编辑器内置的 clang-format 版本不一致导致误报。

## 13. 故障排查

**构建报 “file not found ... codly”**

首次构建需要联网下载 codly。检查网络，或提前在本地跑一次 `typst compile`。

**Typst 报 unclosed delimiter**

通常是文档块里出现了 `*/`，或者行内代码/说明里有未转义的特殊标记。
先看报错指向的 `build/book.typ` 行，再回源文件找对应文字。

**`print.py --check` 编译失败**

看报错的文件和行号。常见原因是模板之间互相 include、缺少类型定义，
或者用了仓库里不存在的头文件。

**中文显示成方框**

本机缺少 Noto CJK 字体。安装 `fonts-noto-cjk`，或在 `book/styles.typ`
里换成本机已有的中文字体。

**PDF 里代码被截断**

检查是否超过 84 列，或者 `@verified` 节的代码过长。分页是自动的，正常
不会裁切；出现裁切先确认没有手动改过 `styles.typ` 的页面高度。

**Release 没更新**

只有 push 到 `main` 才会发布；其他分支只跑构建。另外发布任务会检查
`main` 的最新 commit，旧构建不会覆盖新版本。

## 14. 来源与许可

内容参考了 [hh2048/XCPC](https://github.com/hh2048/XCPC)（GPL-3.0）的打印稿，
已按本仓库的目录结构和排版重写；参考稿本身不随仓库分发。

继续使用或改写来源项目的代码和排版时，保留必要的来源说明，并遵守其许可证。

## 15. 维护原则

1. 一个算法一个文件，自包含，复制即用；
2. 可读性优先，不为了省页压缩代码；
3. 没审完的模板不加 `@verified`；
4. 公共排版规则集中在 `book/`，模板里不写排版；
5. 改完先本地构建，再 push 让 CI 复核。
