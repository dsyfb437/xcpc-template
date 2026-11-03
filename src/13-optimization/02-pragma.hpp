/**
 * @brief 编译指令（只给一段代码开 O3）
 * @complexity -
 * @index -
 * @idea 用 push_options 保存当前选项，用 pop_options 恢复。
 * @usage XCPC_GCC_PUSH_O3
 *   // 把想单独开 O3 的函数定义写在这里
 *   XCPC_GCC_POP_OPTIONS
 * @pitfall 仅 GCC 生效。两个宏在函数外成对使用。先量一下耗时，O3 不一定更快。
 */
#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

#if defined(__GNUC__) && !defined(__clang__)
#define XCPC_GCC_PUSH_O3 \
    _Pragma("GCC push_options") _Pragma("GCC optimize (\"O3\")")
#define XCPC_GCC_POP_OPTIONS _Pragma("GCC pop_options")
#else
#define XCPC_GCC_PUSH_O3
#define XCPC_GCC_POP_OPTIONS
#endif
