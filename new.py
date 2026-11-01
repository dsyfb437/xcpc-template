#!/usr/bin/env python3
"""新建一个板子和对应的测试骨架。

只生成空壳：struct、方法位置、文档块、测试 main。
算法核心留给你自己写——这是这个仓库的规矩。

用法：
    python3 new.py 04-flow dinic --title "最大流（Dinic）"
    python3 new.py 04-flow dinic --title "最大流（Dinic）" \
        --problem https://judge.yosupo.jp/problem/...

生成：
    alg/<章>/<名字>.hpp
    verify/<章>/<名字>.test.cpp
"""
from __future__ import annotations

import argparse
from pathlib import Path
import re
import sys
import tomllib

ROOT = Path(__file__).resolve().parent
BOOK = ROOT / "book.toml"
ALG = ROOT / "alg"
VERIFY = ROOT / "verify"


def pascal(name: str) -> str:
    return "".join(part.capitalize() for part in name.split("_"))


def board_text(title: str, name: str) -> str:
    return f"""/**
 * @brief {title}
 * @complexity TODO
 * @index TODO
 * @status todo
 * @idea TODO：一句话说清核心（不变式 / 为什么对）
 * @usage TODO：`{pascal(name)} obj(...); obj.method(...);`
 * @pitfall TODO：最容易写错的一行（没有就删掉）
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct {pascal(name)} {{
    // TODO: 先写接口，再写实现。
    // 写完把 @status 改成 learning；能讲清楚后再改成 owned。
}};
"""


def test_text(chapter: str, name: str, problem: str | None) -> str:
    problem_line = problem or "TODO"
    note = "" if problem else "// TODO: 填 Library Checker 题号；没有 LC 题就先留 TODO。\n"
    return f"""{note}#define PROBLEM "{problem_line}"

#include <bits/stdc++.h>
#include "../../alg/{chapter}/{name}.hpp"
using namespace std;

int main() {{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    // TODO: 读题 → 调用 {pascal(name)} → 输出
}}
"""


def main() -> int:
    parser = argparse.ArgumentParser(description="新建板子 + 测试骨架")
    parser.add_argument("chapter", help="章目录，例如 04-flow")
    parser.add_argument("name", help="文件名（snake_case），例如 dinic")
    parser.add_argument("--title", help="中文标题，例如：最大流（Dinic）")
    parser.add_argument("--problem", help="Library Checker 题面 URL")
    parser.add_argument("--dry-run", action="store_true", help="只打印，不写文件")
    args = parser.parse_args()

    chapter = args.chapter.strip().strip("/")
    name = args.name.strip()
    if not re.fullmatch(r"[a-z][a-z0-9_]*", name):
        sys.exit("名字只能是 snake_case（小写字母/数字/下划线，字母开头）")

    meta = tomllib.loads(BOOK.read_text(encoding="utf-8"))
    chapters = {entry["dir"]: entry["title"] for entry in meta.get("chapter", [])}
    if chapter not in chapters:
        sys.exit(f"章不存在：{chapter}\n可选：{'、'.join(chapters)}")

    title = args.title or name
    board = ALG / chapter / f"{name}.hpp"
    test = VERIFY / chapter / f"{name}.test.cpp"
    for path in (board, test):
        if path.exists():
            sys.exit(f"已存在：{path.relative_to(ROOT)}")

    board_source = board_text(title, name)
    test_source = test_text(chapter, name, args.problem)

    if args.dry_run:
        for path, source in ((board, board_source), (test, test_source)):
            print(f"--- {path.relative_to(ROOT)} ---")
            print(source)
        return 0

    board.parent.mkdir(parents=True, exist_ok=True)
    test.parent.mkdir(parents=True, exist_ok=True)
    board.write_text(board_source, encoding="utf-8")
    test.write_text(test_source, encoding="utf-8")

    print(f"已生成：")
    print(f"  {board.relative_to(ROOT)}")
    print(f"  {test.relative_to(ROOT)}")
    print()
    print("下一步：")
    print("  1. 读思路，把 struct 里的 TODO 写成你自己的代码；")
    print("  2. 填好测试里的 #define PROBLEM，跑 python3 verify.py --only <题名>；")
    print("  3. 能不看代码讲清楚后，把 @status 改成 owned；")
    print("  4. python3 print.py --status 看进度，python3 print.py 出 PDF。")
    print()
    print("写不下去就先留着 @status todo——书里不会印你看不懂的板子。")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
