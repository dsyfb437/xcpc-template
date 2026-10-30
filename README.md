# xcpc-template

自己用的一些 XCPC 板子，慢慢整理中（

之后大概也会和队友一起补。

[下载打印稿](https://github.com/dsyfb437/xcpc-template/releases/latest/download/template.pdf)

## 目录

- [主函数](src/main.cpp)
- [数据结构](src/ds.cpp)
- [并查集](src/why_you_cannot_write_these.cpp)
- [图论](src/graph.cpp)（还没写）
- [数学](src/math.cpp)
- [字符串](src/string.cpp)
- [凸包与最大三角形](src/cg.cpp)
- [几何](src/cg_.cpp)

## 打印

更新到 GitHub 后会自动生成 PDF，上面那个链接就是。

本地需要 Python 3.11+、Chrome 和中文字体，在仓库目录下运行：

```bash
python -m pip install -r requirements-print.txt
python print.py
```

生成的文件在 `output/pdf/template.pdf`。
