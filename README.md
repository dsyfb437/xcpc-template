# XCPC 模板册

一个板子一个 `.hpp`，文件名和目录用标准英文，文件头写中文说明；
`print.py` 直接排成 A4 打印稿。

## 结构

```
src/                  算法模板（一个算法一个文件）
  01-basics/          基础
  02-trees/           树上问题
  03-graphs/          图论
  04-network-flow/    网络流
  05-number-theory/   数论
  06-geometry/        几何
  08-data-structures/ 数据结构
  09-dynamic-programming/
  10-strings/         字符串
  14-misc/            杂项
book/
  styles.typ          版式（页边距、字体、字号、代码块）
  theme.tmTheme       代码配色
book.toml             书名、章节顺序
print.py              扫描 → 生成 → 调 typst
```

## 用法

```bash
python3 print.py            # 出 output/pdf/template.pdf
python3 print.py --watch    # 改完自动重排
python3 print.py --check    # 编译 + 行宽检查
```

## 文档块

每个文件开头是一段 `/** ... */`，`print.py` 按 `@标签 值` 解析：

```cpp
/**
 * @brief 提交模板
 * @complexity -
 * @index -
 * @idea 现场赛提交骨架：调试宏、多测开关、板子区。
 * @usage 改 `solve()` 和 `namespace lib` 里的板子区。
 * @verified
 * @print-boilerplate
 */
```

### 标签

| 标签 | 作用 | 显示位置 |
| --- | --- | --- |
| `@brief` | 小节标题 | 标题；不写则用文件名 |
| `@complexity` | 复杂度 | 标题右端，和 `@index` 合并 |
| `@index` | 下标约定 | 同上 |
| `@idea` | 一句话核心 | 字段表「核心」 |
| `@usage` | 用法 | 字段表「用法」 |
| `@pitfall` | 坑 | 字段表「坑」 |
| `@verified` | 开关：有它才印代码 | 不显示 |
| `@print-boilerplate` | 开关：保留 `#include` / `using` | 不显示 |

### 顺序

解析**不要求顺序**，标签写前写后都一样。

显示顺序是固定的：

1. 标题右端：复杂度 · 下标；
2. 字段表：依赖 → 核心 → 用法 → 坑；
3. 自由段落：`用途：`、`做法：`、`例：` 等，按源文件里的先后排。

建议按这个顺序写，方便人读：

```cpp
/**
 * @brief ...
 * @complexity ...
 * @index ...
 * @idea ...
 * @usage ...
 * @pitfall ...
 * @verified
 * @print-boilerplate
 *
 * 用途：...
 * 例：...
 */
```

### 细节

- `@complexity -`、`@index -` 等同于不写，可以省略；
- `@verified`、`@print-boilerplate` 是开关，不写值；
- 不认识的标签会被静默忽略，拼错不会报错；
- 文档块必须在文件最开头，且里面不能再出现 `*/`（会提前结束注释）；
- 自由说明里，`用途：`、`做法：`、`用法：`、`坑：`、`例：` 开头会变成字段行，空行分段；
- `@status` 是旧字段（todo/learning/owned/unreviewed），会过滤整节，新板子不要写。

## 验证标记

文件头加 `@verified` 才把代码印进书；没标记只印说明。
**现在全部未标记**，所以 PDF 里只有标题和说明。

审完一个板子，在它的文档块里加一行：

```cpp
/**
 * @brief 点分治
 * @complexity O(n log n)
 * @index 点 1-based
 * @verified
 * ...
 */
```

## 写一个板子

1. 在 `src/<章>/` 新建 `NN-英文名.hpp`；
2. 开头按「文档块」一节写 `/** ... */`；
3. 不加 `@verified` 只排说明，加了才排代码。

## 命名约定

- 目录、文件名：小写英文 + 连字符，例如 `05-centroid-decomposition.hpp`；
- `NN-` 前缀只用来控制章节内顺序；
- 中文标题写在 `@brief` 里，文件名不用中文，避免跨平台编码问题。

## 排版

- 每章从新页开始；
- 加了 `@verified` 的节从新页开始，没验证的说明连续排；
- 一节多长都行，超过一页自然续页，不裁切；
- 常用版式参数在 `book/styles.typ` 顶部：`body-size`、`code-size` 和页边距。

## 来源

内容参考了 [hh2048/XCPC](https://github.com/hh2048/XCPC)（GPL-3.0）的打印稿，已按本仓库结构改写；参考稿不随仓库分发。

## 依赖

每个板子都是自包含的，不 `#include` 其他板子；复制走一个文件就能用。
