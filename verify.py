#!/usr/bin/env python3
"""跑 verify/**/*.test.cpp 的 Library Checker 验证。

每个 test 文件第一行是

    #define PROBLEM "https://judge.yosupo.jp/problem/unionfind"

工具按 URL 找到题包里的题目，拉取、生成官方数据，编译 test 文件，
逐组运行并和官方输出比对（题目带 checker 的用 checker）。

用法：
    python3 verify.py              # 全部
    python3 verify.py --only lca scc
    python3 verify.py --setup-only # 只拉题包
"""
from __future__ import annotations

import argparse
import os
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import sys
import time

ROOT = Path(__file__).resolve().parent
VERIFY = ROOT / "verify"
LC = ROOT / "third_party" / "library-checker-problems"
BUILD = ROOT / "build" / "verify"
GIT = ["git", "-c", "http.proxy=", "-c", "https.proxy="]

def git_env() -> dict[str, str]:
    """环境里的代理可能是坏的，git 只认 -c 不够，还得清掉这些变量。"""
    env = dict(os.environ)
    for key in list(env):
        if key.lower() in {"http_proxy", "https_proxy", "all_proxy"}:
            env.pop(key)
    return env
PROBLEM_RE = re.compile(r'^\s*#define\s+PROBLEM\s+"([^"]+)"', re.M)
CASE_TIMEOUT = 30


def collect() -> list[tuple[str, Path]]:
    found = []
    skipped = []
    for path in sorted(VERIFY.rglob("*.test.cpp")):
        match = PROBLEM_RE.search(path.read_text(encoding="utf-8"))
        if not match:
            raise SystemExit(f"{path.relative_to(ROOT)} 缺少 #define PROBLEM")
        name = match.group(1).rstrip("/").split("/")[-1]
        if name == "TODO":
            skipped.append(path)
            continue
        found.append((name, path))
    for path in skipped:
        print(f"跳过 {path.relative_to(ROOT)}：还没填 #define PROBLEM")
    return found


def ensure_repo(needed: list[str]) -> dict[str, str]:
    """拉取题包元数据，返回 题目名 -> 目录。"""
    if not LC.exists():
        LC.parent.mkdir(parents=True, exist_ok=True)
        print("拉取 Library Checker 题包…", flush=True)
        # 网络偶尔会抖，克隆重试三次
        for attempt in range(3):
            try:
                subprocess.run(
                    GIT + [
                        "clone", "--filter=blob:none", "--no-checkout", "--depth", "1",
                        "https://github.com/yosupo06/library-checker-problems.git", str(LC),
                    ],
                    check=True,
                    env=git_env(),
                )
                break
            except subprocess.CalledProcessError:
                if attempt == 2:
                    raise
                print("克隆失败，重试…", flush=True)
                time.sleep(3)
        subprocess.run(GIT + ["sparse-checkout", "init", "--cone"], cwd=LC, check=True, env=git_env())

    tree = subprocess.run(
        GIT + ["ls-tree", "-r", "--name-only", "HEAD"],
        cwd=LC, capture_output=True, text=True, check=True, env=git_env(),
    ).stdout
    mapping = {}
    for line in tree.splitlines():
        if line.endswith("/info.toml"):
            directory = line[: -len("/info.toml")]
            mapping[directory.rsplit("/", 1)[-1]] = directory

    missing = [name for name in needed if name not in mapping]
    if missing:
        raise SystemExit(f"题包里找不到：{'、'.join(missing)}")

    dirs = sorted({mapping[name] for name in needed})
    subprocess.run(GIT + ["sparse-checkout", "set", *(["common"] + dirs)], cwd=LC, check=True, env=git_env())
    subprocess.run(GIT + ["checkout"], cwd=LC, check=True, env=git_env())
    return mapping


def has_cases(directory: Path) -> bool:
    # 有些用例输出本来就是空的，所以用生成完成的标记，而不是看文件大小
    if not (directory / ".generated").exists():
        return False
    ins = list((directory / "in").glob("*.in"))
    outs = list((directory / "out").glob("*.out"))
    return bool(ins) and len(ins) == len(outs)


def generate(name: str, directory: Path, force: bool = False) -> None:
    if has_cases(directory) and not force:
        return
    print(f"生成 {name} 的官方数据…", flush=True)
    (directory / ".generated").unlink(missing_ok=True)
    for leftover in ("in", "out"):
        shutil.rmtree(directory / leftover, ignore_errors=True)
    command = (
        "ulimit -s unlimited 2>/dev/null; "
        f"exec {shlex.quote(sys.executable)} generate.py {shlex.quote(str(directory.relative_to(LC)) + '/info.toml')}"
    )
    if subprocess.run(["bash", "-c", command], cwd=LC).returncode != 0:
        raise RuntimeError(f"{name}: generate.py 失败")
    (directory / ".generated").write_text("", encoding="utf-8")


def verify_one(name: str, path: Path, directory: Path) -> tuple[int, int, float]:
    generate(name, directory)
    BUILD.mkdir(parents=True, exist_ok=True)
    binary = BUILD / path.stem
    result = subprocess.run(
        ["g++", "-std=c++20", "-O2", "-o", str(binary), str(path)],
        capture_output=True, text=True,
    )
    if result.returncode != 0:
        raise RuntimeError(f"{path.name} 编译失败\n{result.stderr[-600:]}")

    cases = sorted((directory / "in").glob("*.in"))
    started = time.time()
    passed = 0
    for case in cases:
        expected = directory / "out" / f"{case.stem}.out"
        output = BUILD / f"{path.stem}.{case.stem}.out"
        with case.open("rb") as source, output.open("wb") as sink:
            try:
                run = subprocess.run(
                    ["bash", "-c", 'ulimit -s unlimited 2>/dev/null; exec "$0"', str(binary)],
                    stdin=source, stdout=sink, stderr=subprocess.PIPE, timeout=CASE_TIMEOUT,
                )
            except subprocess.TimeoutExpired:
                print(f"  ✗ {path.name} / {case.stem}: 超时")
                continue
        ok = False
        if run.returncode == 0:
            checker = directory / "checker"
            if checker.exists():
                check = subprocess.run(
                    [str(checker), str(case), str(expected), str(output)],
                    capture_output=True, text=True,
                )
                ok = check.returncode == 0
            else:
                ok = output.read_bytes() == expected.read_bytes()
        if ok:
            passed += 1
        else:
            print(f"  ✗ {path.name} / {case.stem}")
    return passed, len(cases), time.time() - started


def main() -> int:
    parser = argparse.ArgumentParser(description="Library Checker 验证")
    parser.add_argument("--only", nargs="*", help="只跑这几题")
    parser.add_argument("--setup-only", action="store_true")
    parser.add_argument("--force", action="store_true", help="强制重新生成数据")
    args = parser.parse_args()

    all_tests = collect()
    if not all_tests:
        print("没有可跑的 LC 测试")
        return 0
    tests = all_tests
    if args.only:
        tests = [item for item in all_tests if item[0] in args.only]
        if not tests:
            print("没有匹配的题目")
            return 1

    # 始终保留所有题目的目录，否则 --only 会把别的题的数据删掉
    mapping = ensure_repo([name for name, _ in all_tests])
    if args.setup_only:
        print(f"题包就绪：{LC}")
        return 0

    total_pass = total_cases = 0
    failures = []
    for name, path in tests:
        try:
            if args.force:
                generate(name, LC / mapping[name], force=True)
            passed, cases, seconds = verify_one(name, path, LC / mapping[name])
        except Exception as error:  # noqa: BLE001
            print(f"✗ {name}: {error}")
            failures.append(name)
            continue
        total_pass += passed
        total_cases += cases
        print(f"{'✓' if passed == cases else '✗'} {name:24s} {passed}/{cases}  {seconds:5.1f}s")
        if passed != cases:
            failures.append(name)

    print(f"\n合计 {total_pass}/{total_cases} 组官方数据")
    if failures:
        print("没过：" + "、".join(failures))
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
