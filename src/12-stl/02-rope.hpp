/**
 * @brief rope（剪下一段，插到别处）
 * @complexity 平衡时索引和切接通常为 O(log n)
 * @index 剪切范围为 [l,r)，pos 按剪完后的序列编号
 * @idea 用 substr 取一段，再 erase 和 insert。rope 会共享未修改的片段。
 * @usage __gnu_cxx::rope<char> s("abcdef");
 *   rope_move(s, 1, 3, 3);  // 剪下 bc，插到 e 后，得到 adebcf
 *   char c = s[0];
 * @pitfall GNU libstdc++ 扩展。substr、erase 的第二个参数是长度。
 */
#pragma once
#include <bits/stdc++.h>
#include <ext/rope>
using namespace std;
using ll = long long;

template <class T>
void rope_move(__gnu_cxx::rope<T> &s, size_t l, size_t r, size_t pos) {
    auto block = s.substr(l, r - l);
    s.erase(l, r - l);
    s.insert(pos, block);
}
