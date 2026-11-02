# 维护说明

这份文档说明当前仓库的结构、文档块格式、审阅标记、构建与发布流程。
`README.md` 面向使用者，这里面向改仓库的人。

## 1. 目录结构

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
  styles.typ               页面与排版样式
  theme.tmTheme            代码配色

book.toml                  书名、章节顺序
print.py                   扫描 → 生成 → 调 Typst
.github/workflows/check.yml  CI：检查、构建、发 Release
.clang-format              代码格式规则
```

`build/`、`output/` 是生成目录，已在 `.gitignore` 里；`.vscode/` 是个人的
VS Code 配置，也不进仓库。

## 2. 文件与命名

- 目录、文件名用小写英文 + 连字符：`05-centroid-decomposition.hpp`；
- `NN-` 前缀只控制同一章内的顺序，不参与算法含义；
- 中文标题写在 `@brief` 里，文件名不用中文；
- 一个文件只放一个算法，文件名和内容对得上。

`book.toml` 里的 `dir` 必须和 `src/` 下的目录名一致；章节顺序以
`book.toml` 为准，不依赖文件系统遍历顺序。

## 3. 自包含

所有模板必须能单独复制走：

- 不 `#include` 仓库里的其他模板；
- 需要的小结构、辅助函数直接写在当前文件里；
- 不建 `utils.hpp` 这类公共依赖；
- 复制一个 `.hpp` 到比赛代码里就能编译。

重复少量代码是可以接受的，换依赖省不了多少行，反而容易在赛场上漏带文件。

## 4. 文档块

每个文件开头必须是一段 `/** ... */`，`print.py` 按 `@标签 值` 解析。

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

### 4.1 标签

| 标签 | 作用 | 打印位置 |
| --- | --- | --- |
| `@brief` | 小节标题 | 标题；不写则用文件名 |
| `@complexity` | 复杂度 | 标题右端 |
| `@index` | 下标约定 | 标题右端，和复杂度用 `·` 隔开 |
| `@idea` | 一句话核心 | 字段表「核心」 |
| `@usage` | 用法 | 字段表「用法」 |
| `@pitfall` | 坑 | 字段表「坑」 |
| `@verified` | 是否打印代码 | 不显示 |
| `@print-boilerplate` | 保留 `#include` / `using` | 不显示 |

`@complexity` 和 `@index` 写 `-` 等同于不写。`@verified`、
`@print-boilerplate` 是开关，不写值。

### 4.2 顺序

解析不要求顺序，标签写前写后都一样。

显示顺序固定：

1. 标题右端：复杂度 · 下标；
2. 字段表：依赖 → 核心 → 用法 → 坑；
3. 自由说明：`用途：`、`做法：`、`用法：`、`坑：`、`例：` 开头的行，
   按源文件里的先后排列。

建议的书写顺序就是上面示例：先 `@brief` / `@complexity` / `@index`，
再 `@idea` / `@usage` / `@pitfall`，最后开关和自由说明。

### 4.3 手动换行

一个标签的内容要手动分行时，在标签下面缩进两个空格以上：

```cpp
 * @usage 第一行
 *        第二行
```

续行会并进上一个标签，渲染时保留换行。空行或新标签会结束续行；
没有缩进的行仍然按自由说明处理。

### 4.4 数学

- `@complexity` 里的 `O(...)` / `Θ(...)` 会自动转成数学；
- 正文里用 `$...$` 写 Typst 数学，例如 `$x^2 >= n$`、`$O(log n)$`；
- 这是 Typst 语法，不是 LaTeX：写 `frac(a, b)`、`sqrt(n)`，不要写
  `\frac`、`\sqrt`；
- 反引号里是代码，`$` 不会当数学解析。

### 4.5 细节

- 文档块必须在文件最开头；
- 文档块内部不能再出现 `*/`，否则会提前结束注释；
- 未识别的标签会被静默忽略，拼错不会报错；
- `@brief` 不写时用文件名兜底。

## 5. 验证标记

`@verified` 决定代码是否进入打印稿：

| 状态 | 打印说明 | 打印代码 |
| --- | --- | --- |
| 没有 `@verified` | 是 | 否 |
| 有 `@verified` | 是 | 是 |

当前没有自动判题，验证靠人工。审完一个模板，再在文档块里加 `@verified`。
未验证的模板可以提交，只是 PDF 里不会出现代码。

审阅时至少检查：

- 思路和复杂度是否正确；
- 下标约定是否写清楚；
- 边界、空输入、特殊图是否处理；
- 是否自包含、复制走能不能编译；
- `@pitfall` 里的限制是否真的成立。

## 6. 保留前置声明

默认情况下，打印代码时会省略 `#include`、`#pragma once`、`using` 这类前置。

如果希望打印稿保留原始代码里的：

- `#include`
- `using`

就在文档块里加：

```cpp
 * @print-boilerplate
```

适合需要整段复制走的模板。

## 7. 构建

环境：Python 3、Typst。首次构建会在线下载 codly，需要网络。

```bash
python3 print.py            # 生成 output/pdf/template.pdf
python3 print.py --watch    # 改完自动重排
python3 print.py --check    # 编译 + 行宽检查
```

构建后重点看：

- 章节顺序、标题、字段是否完整；
- 代码有没有超宽（`--check` 会报）；
- 加了 `@verified` 的节是否从新页开始；
- 未验证的说明是否连续排版、没有大片空白。

## 8. 排版

- 每章从新页开始；
- 有 `@verified` 的节从新页开始；
- 没有 `@verified` 的节连续排；
- 单节超长时自然续页，不裁切。

常用参数在 `book/styles.typ` 顶部：`body-size`、`code-size`、页边距。
代码配色在 `book/theme.tmTheme`。优先改统一参数，不要在模板里写排版。

## 9. CI 与发布

`.github/workflows/check.yml` 在每次 push 时运行：

1. 安装 Typst 和中文字体；
2. `python3 print.py --check`；
3. `python3 print.py`；
4. 上传 `template.pdf` artifact；
5. 如果推的是 `main`，创建 Release。

Release 的 tag 是 `print-<sha12>`，标题是 `打印稿 · YYYY-MM-DD · <sha7>`，
附件是 `template.pdf`。只有 `main` 会发布，其他分支只跑构建。

## 10. 格式化

`.clang-format` 是共享规则，VS Code 的 C/C++ 扩展会读取它；
`.vscode/` 不进仓库，个人设置自己保留。

手动格式化：

```bash
npx --yes clang-format@1.8.0 -i src/*/*.hpp
```

CI 目前**不检查格式**，只在本地/编辑器里做，避免 CI 和编辑器版本不一致
造成误报。

## 11. 来源

内容参考了 [hh2048/XCPC](https://github.com/hh2048/XCPC)（GPL-3.0）的打印稿，
已按本仓库结构改写；参考稿不随仓库分发。

## 12. 维护原则

1. 一个算法一个文件，自包含，复制即用；
2. 可读性优先，不为了省页压缩代码；
3. 没审完的模板不加 `@verified`；
4. 公共排版规则集中在 `book/`，模板里不写排版；
5. 改完先本地 `print.py`，再 push 让 CI 复核。
