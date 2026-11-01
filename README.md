# XCPC 模板册

一个板子一个 `.hpp`，文件名和目录用标准英文，文件头写中文说明；`print.py` 直接排成 A4 打印稿。

## 结构

```
algorithms/          算法模板
  01-basics/         基础
  02-trees/          树上问题
  03-graphs/         图论
  04-network-flow/   网络流
  05-number-theory/  数论
  06-geometry/       几何
  08-data-structures/数据结构
  09-dynamic-programming/
  10-strings/        字符串
  14-misc/           杂项
book/
  styles.typ         版式（页边距、字体、字号、代码块）
  theme.tmTheme      代码配色
  vendor/            codly 依赖
book.toml            书名、章节顺序
print.py             扫描 → 生成 → 调 typst
```

## 用法

```bash
python3 print.py            # 出 output/pdf/template.pdf
python3 print.py --watch    # 改完自动重排
python3 print.py --check    # 编译 + 行宽检查
```

## 写一个板子

1. 在 `algorithms/<章>/` 新建 `NN-英文名.hpp`，开头写文档块：

```cpp
/**
 * @brief 并查集
 * @complexity 近似 O(1)
 * @index 下标 1-based
 *
 * 用途：维护连通块。
 * 用法：`DSU dsu(n); dsu.unite(u, v);`
 * 坑：下标从 1 开始。
 */
```

2. `python3 print.py` 看效果。

## 命名约定

- 目录、文件名：小写英文 + 连字符，例如 `05-centroid-decomposition.hpp`；
- `NN-` 前缀只用来控制章节内的顺序；
- 中文标题写在 `@brief` 里，文件名不用中文，避免跨平台编码问题。

## 排版

- 每章、每节都从新页开始；
- 一节多长都行，超过一页自然续页，不裁切；
- 常用版式参数在 `book/styles.typ` 顶部：`body-size`、`code-size` 和页边距。

## 依赖

每个板子都是自包含的，不 `#include` 其他板子；复制走一个文件就能用。
