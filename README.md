# XCPC 模板册

一个板子一个中文名 `.hpp`，文档写在文件头，`print.py` 直接排出 A4 打印稿。

## 结构

```
板子/           算法，按章分目录，文件名就是标题
排版/
  styles.typ    版式（页边距、字体、字号、代码块）
  theme.tmTheme 代码配色
  vendor/       codly（代码行号）依赖
book.toml       书名、章节顺序
print.py        扫描 → 生成 → 调 typst
```

## 用法

```bash
python3 print.py            # 出 output/pdf/template.pdf
python3 print.py --watch    # 改完自动重排
python3 print.py --check    # 编译 + 行宽检查
```

## 写一个板子

1. 在 `板子/<章>/` 新建 `NN-标题.hpp`，开头写文档块：

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

## 排版约定

- 每章从新页开始，每节也从新页开始；
- 一节多长都行，超过一页自然续页，不裁切；
- 想调版式，改 `排版/styles.typ` 顶部的 `body-size`、`code-size` 和边距。
