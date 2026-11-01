# 代码与文档风格

这份文件是给“以后加板子”用的，不是给现在这 49 个板子追认的。
新板子按这里写；老板子在你重新看懂、重写时再逐步迁移。

## 目标

- 书里只放**你能讲清楚**的板子。
- 一个板子 = 一个文件 = 一个明确接口。
- 宁可短、直白，不炫技；贴上去改两行就能用。

## 状态

每个板子的文档块里有 `@status`：

| 状态 | 含义 | 是否印进 PDF |
|---|---|---|
| `todo` | 还没写 / 还没学 | 否 |
| `learning` | 有代码，但你还讲不清 | 否 |
| `owned` | 能不看代码讲清核心，能默写 | 是 |
| `unreviewed` | 老板子，还没分类（默认） | 是 |

```bash
python3 print.py --status   # 看每个板子的状态和 LC 覆盖
python3 print.py            # 只印 owned + unreviewed
python3 print.py --all      # 连 todo/learning 一起印（复习用）
```

新板子一律从 `todo` 开始。AI 生成完直接标 `owned` 是不允许的。

## 文件

```
alg/<章>/<snake_case>.hpp       板子
verify/<章>/<name>.test.cpp     LC 测试（可选）
```

- 依赖写 `#include "..."`，同章依赖会自动排在前面；跨章依赖目前不保证顺序，尽量不要。
- 打印会剥掉 `#pragma once` / `#include` / `using namespace std;` / `using ll = long long;`；
  提交模板那种要完整 include 的，加 `@print-boilerplate`。
- 文件名用 snake_case；一个板子一个文件；不要建 `utils.hpp` 这种大杂烩。

## 代码

- 目标标准 C++20，但别用冷门 C++20 特性；评测机 C++17 也要能过。
- 一个文件一个 `struct`，或者一组 `inline` 函数。字段默认 public，比赛模板不搞 getter/setter。
- 优先 STL：`vector`、`array`、`priority_queue`、`queue`、`stack`、`set`、`map`、
  `unordered_map`、`tuple`、`pair`、`optional`、`string`。
- 结构化绑定、`auto`、范围 for、lambda 都可以用。**递归 lambda 不推荐**（`auto &&self`
  可读性差），能用普通递归/迭代就用普通的。
- **回调 API 不推荐**（例如 `path(u, v, f)`）。优先返回 `vector`，或者让调用方自己写循环；
  除非回调能明显省内存/时间。
- 下标默认 1-based；字符串/几何/位运算 0-based。每个板子必须写 `@index`。
- 类型统一 `ll = long long`；乘法注意 `1LL *`；坐标叉积接近 1e18 时用 `__int128`。
- 命名短但别猜谜：`n, m, u, v, w, fa, dep, sz, vis` 可以；不要 `a1, a2, tmp2`。
- 4 空格缩进；左花括号不换行；行宽 ≤ 84；嵌套必须加大括号。
- 注释只写“为什么 / 不变式 / 坑”，不写“这里遍历”。代码能自解释就不写注释。
- 不手动 `new/delete`；不写裸指针；不写平台相关代码（`__int128` 除外，XCPC 默认 GCC）。
- 需要卡常时单独开 `13-const/`，不要把卡常技巧混进普通板子。

## 文档块

每个文件开头：

```cpp
/**
 * @brief 一句话标题
 * @complexity O(...)
 * @index 下标约定
 * @status todo
 * @idea 一句话核心：为什么对 / 不变式
 * @usage `Struct s(n); s.method(...);`
 * @pitfall 最容易写错的一行（可选）
 */
```

- `@brief` 是标题；`@complexity` 排在小节标题右端；`@index` 单独一行。
- `@idea` / `@usage` / `@pitfall` 会排在正文，各限 1–2 行，不写长散文。
- 没有 `@index` 的板子会被 `--status` 标出来；提交模板/随机数这类可以写 `-`。
- 老板子里的“用途/做法/坑/例”自由文本还能用，但新板子优先用上面的标签。

## 测试

- 有 Library Checker 题：测试第一行写
  `#define PROBLEM "https://judge.yosupo.jp/problem/..."`，然后
  `python3 verify.py --only <题名>`。
- 没 LC 题：在测试里写暴力/随机对拍 + `assert`，或者先留 `TODO`；
  `verify.py` 会跳过 `TODO` 的测试。
- 标 `owned` 之前至少满足一个：LC 全过 / 自测过 / 人工对拍过。
- 测试文件名和板子文件名尽量一致；一个板子多个测试可以加后缀
  （例如 `euler_trail_directed.test.cpp`）。

## 新增流程

1. `python3 new.py <章> <名字> --title "..."`（会生成 `todo` 空壳 + 测试骨架）
2. 读思路，自己写核心。写不动就问，不要直接让 AI 生成整段。
3. 填 `#define PROBLEM`，跑 `python3 verify.py --only ...`
4. 能不看代码讲清楚后，把 `@status` 改成 `owned`
5. `python3 print.py --status` 看进度，`python3 print.py` 出 PDF

## AI 边界

- AI 可以：生成脚手架/测试骨架、查 LC 题号、讲思路、review、改排版脚本。
- AI 不可以：一次性生成几十个板子、替你标 `owned`、把没验证的代码放进书里。
- 任何 AI 生成的算法核心，必须你逐行看懂、能默写，才配 `owned`。

## 排版（当前约定）

- 说明文字用 sans、左对齐；不要两端对齐（中英混排会把字距拉得很开）。
- 说明按字段排：下标 / 用途 / 核心 / 用法 / 坑 / 例 / 依赖。
- 例子用等宽字体，放在 `example` 里，不要混在正文里。
- 一个板子尽量一页；长板跨页时页眉保留当前小节名，不允许只有一两行的孤页。
- 版式只改 `book/styles.typ`，先读 `book/README.md`，单独提交；
  不要和板子改动混在一起。
