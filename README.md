# xcpc-template

个人在用的 XCPC 算法竞赛模板。板子按主题拆在 `src/` 里，推上去后由 GitHub Actions 自动合成可打印的 PDF。

[下载最新打印稿](https://github.com/dsyfb437/xcpc-template/releases/latest/download/template.pdf)

## 目录

| 文件 | 内容 |
| --- | --- |
| [`src/main.cpp`](src/main.cpp) | 单组/多组数据的 main 骨架 |
| [`src/ds.cpp`](src/ds.cpp) | 数据结构 |
| [`src/why_you_cannot_write_these.cpp`](src/why_you_cannot_write_these.cpp) | 并查集等“手写容易错”的短板子 |
| [`src/graph.cpp`](src/graph.cpp) | 图论（还没写） |
| [`src/math.cpp`](src/math.cpp) | 数学 |
| [`src/string.cpp`](src/string.cpp) | 字符串 |
| [`src/cg.cpp`](src/cg.cpp) | 凸包、最大三角形 |
| [`src/cg_.cpp`](src/cg_.cpp) | 计算几何综合 |

## 怎么用

- 比赛时按需把对应片段复制进自己的代码；`main.cpp` 只是骨架，不依赖其他文件。
- 不是每个 `src/*.cpp` 都能单独编译（有些只放函数/结构体），复制后按需补 `main`。
- 新增或修改板子直接改 `src/`，不要改 `output/`（构建产物，已被忽略）。

## 本地生成打印稿

需要 Python 3.11+、Chrome/Chromium 和中文字体：

```bash
python3 -m pip install -r requirements-print.txt
python3 print.py
```

产物在 `output/pdf/template.pdf`，同时会有 `output/template.md` 和 `output/template.html`。

## 自动生成与检查

- 每次 push / PR 都会跑 `.github/workflows/check.yml`：安装打印依赖、跑 `tests/`、生成 PDF。
- `main` 上的新提交会发布到对应 release，首页的“下载最新打印稿”始终指向最新版。
- 本地也可以先跑测试：

```bash
python3 -m unittest discover -s tests
```

## 说明

- 这是个人模板，不保证覆盖所有题型；`graph.cpp` 目前为空。
- 没有指定开源许可证；如果要复用，先开 issue 问一下。
