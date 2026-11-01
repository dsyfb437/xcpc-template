# xcpc-template

一个**可维护**的 XCPC 模板框架：一个算法一个 `.hpp`，文档块 + 测试 + 一键 PDF。

重点不是“一次性生成整本书”，而是**每加一个板子，你都知道它为什么对**：

- `todo` / `learning` 的板子不进书；
- 只有你能讲清楚的 `owned` 板子才会印；
- 没验证的代码不标 `owned`。

## 快速开始

```bash
python3 new.py 11-game sg --title "SG 函数"   # 生成 todo 空壳，核心自己写
python3 print.py --status                     # 看状态：todo 不进书
python3 verify.py --only lca                  # 跑一个现成测试，确认环境
python3 print.py                              # 出 PDF
```

## 目录

```
alg/          板子（.hpp / .cpp），文档块写在文件头
verify/       测试（.test.cpp，第一行是 LC 题号）
book/         排版样式（Typst）
new.py        新建板子 + 测试骨架（只生成空壳）
print.py      alg/ → output/pdf/template.pdf
verify.py     verify/ → 自动跑 Library Checker
STYLE.md      代码/文档风格
AGENTS.md     给 AI / 协作者的边界
```

## 状态：书里只放你看得懂的

每个板子的文档块里有 `@status`：

| 状态 | 含义 | 进 PDF |
|---|---|---|
| `todo` | 还没写 / 还没学 | 否 |
| `learning` | 有代码，但你还讲不清 | 否 |
| `owned` | 能不看代码讲清核心、能默写 | 是 |
| `unreviewed` | 老板子，还没分类（默认） | 是 |

```bash
python3 print.py --status   # 每个板子的状态、LC 覆盖、行数、空章、缺 @index
python3 print.py            # 只印 owned + unreviewed
python3 print.py --all      # 连 todo/learning 一起印（复习用）
```

老板子默认 `unreviewed`，不会因为引入状态就消失。
你确认一个板子看懂了，就把它改成 `owned`；看不懂的改成 `learning`，它就不会占书页。

## 加一个算法（人工流程）

1. `python3 new.py <章> <名字> --title "中文标题"` 生成 `todo` 空壳 + 测试骨架；
2. 自己写核心。AI 可以讲思路、给接口、review，但**不直接给整段答案**；
3. 填测试里的 `#define PROBLEM`，跑 `python3 verify.py --only <题名>`；
4. 能不看代码讲清楚后，把 `@status` 改成 `owned`；
5. `python3 print.py --status` 看进度，`python3 print.py` 出 PDF。

写不动就先留着 `todo`。书里不印你看不懂的板子。

## 文件格式

```cpp
/**
 * @brief 并查集
 * @complexity 近似 O(1)
 * @index 下标 1-based
 * @status todo
 * @idea 路径压缩把树高摊平，按大小合并不让树长歪。
 * @usage `DSU dsu(n); dsu.unite(u, v);`
 * @pitfall 多测要重新构造，别忘了。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct DSU { ... };
```

- `@brief` 是标题；`@complexity` 排在小节标题右端；`@index` 单独一行；
- `@idea` / `@usage` / `@pitfall` 会排进正文，各限 1–2 行；
- 老板子里的“用途/做法/坑/例”自由文本仍然兼容；
- 需要完整 include 的提交模板加 `@print-boilerplate`，打印时不会剥掉。

## 打印

```bash
python3 print.py            # 出 PDF
python3 print.py --watch    # 改完自动重排
python3 print.py --check    # 编译 + 行宽检查
python3 print.py --status   # 状态表
```

A4 单面、左边装订（左边距 20mm）、黑白优先。代码 7.8pt / 84 列带行号，
说明是字段卡片，代码是浅底代码卡片；当前 49 个板子 / 39 页。版式在
[`book/styles.typ`](book/styles.typ)，变量和规则见 [`book/README.md`](book/README.md)，
配色在 [`book/theme.tmTheme`](book/theme.tmTheme)；双面打印把
[`book.toml`](book.toml) 里的 `duplex` 改成 `true`。

## 验证

```bash
python3 verify.py           # 跑全部 LC 验证
python3 verify.py --only lca scc
python3 verify.py --setup-only
```

`verify.py` 会稀疏拉取 [library-checker-problems](https://github.com/yosupo06/library-checker-problems)
里用到的题目，跑题目自带的 `generate.py` 生成官方数据，再编译 `verify/` 下的 test
逐组比对（题目带 checker 的用官方 checker）。

目前 18 个 test 覆盖 17 个板子、425 组官方数据；其余板子暂时只有 `print.py --check`
的编译检查。没有 LC 题的板子，请在测试里写暴力/随机对拍，`TODO` 测试会被自动跳过。

## 依赖

Python 3.11+、g++、git，以及 [Typst](https://typst.app)（单文件二进制，
放 PATH 或 `~/.local/bin/typst`）。

## 维护约定

- 一个 commit 一个板子 / 一个工具改动；版式改动单独提交；
- 新增板子走 `new.py`，不要批量生成；
- 不替别人标 `owned`，不把没验证的代码放进书里；
- 详细风格见 [STYLE.md](STYLE.md)，AI 协作边界见 [AGENTS.md](AGENTS.md)。
