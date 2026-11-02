# 模板目录树

```
src/
├── 01-basics/  # 基础算法（12）
│   ├── 00-template.cpp  # 提交模板
│   ├── 01-binary-search.hpp  # 整数二分
│   ├── 02-real-binary-search.hpp  # 实数二分
│   ├── 03-ternary-search.hpp  # 三分
│   ├── 04-discretization.hpp  # 离散化
│   ├── 05-prefix-sum.hpp  # 前缀和 / 差分
│   ├── 06-merge-sort.hpp  # 归并排序 / 逆序对
│   ├── 07-two-pointers.hpp  # 双指针 / 尺取
│   ├── 08-binary-lifting.hpp  # 倍增
│   ├── 09-divide-conquer.hpp  # 递归与分治
│   ├── 10-greedy.hpp  # 贪心
│   └── 11-construction.hpp  # 构造
├── 02-trees/  # 树上问题（16）
│   ├── 01-tree-diameter.hpp  # 树的直径
│   ├── 02-tree-centroid.hpp  # 树的重心
│   ├── 03-lca.hpp  # 最近公共祖先（倍增）
│   ├── 04-hld.hpp  # 重链剖分
│   ├── 05-centroid-decomposition.hpp  # 点分治（重心分解）
│   ├── 06-path-intersection.hpp  # 树上路径交
│   ├── 07-dsu-on-tree.hpp  # 树上启发式合并（DSU on tree）
│   ├── 08-tree-difference.hpp  # 树上差分
│   ├── 09-auxiliary-tree.hpp  # 虚树
│   ├── 10-tree-hash.hpp  # 树哈希
│   ├── 11-long-chain-decomposition.hpp  # 长链剖分
│   ├── 12-tree-center.hpp  # 树的中心
│   ├── 13-tree-decomposition.hpp  # 树分块
│   ├── 14-dynamic-tree-divide.hpp  # 点分树 / 动态点分治
│   ├── 15-tree-ahu.hpp  # AHU 树同构
│   └── 16-tree-random-walk.hpp  # 树上随机游走
├── 03-graphs/  # 图论（37）
│   ├── 01-shortest/  # 最短路（6）
│   │   ├── 01-dijkstra.hpp  # Dijkstra（堆优化）
│   │   ├── 02-difference-constraints.hpp  # 差分约束
│   │   ├── 03-spfa.hpp  # Bellman-Ford / SPFA / 判负环
│   │   ├── 04-floyd.hpp  # Floyd 全源最短路
│   │   ├── 05-kth-shortest.hpp  # k 短路
│   │   └── 06-mod-shortest.hpp  # 同余最短路
│   ├── 02-spanning-tree/  # 生成树（5）
│   │   ├── 01-kruskal.hpp  # 最小生成树（Kruskal）
│   │   ├── 02-directed-mst.hpp  # 最小树形图（朱刘）
│   │   ├── 03-prim.hpp  # Prim 最小生成树
│   │   ├── 04-steiner-tree.hpp  # 斯坦纳树
│   │   └── 05-min-diameter-st.hpp  # 最小直径生成树
│   ├── 03-connectivity/  # 连通性（12）
│   │   ├── 01-scc.hpp  # 强连通分量（Tarjan）
│   │   ├── 02-topological-sort.hpp  # 拓扑排序（DFS）
│   │   ├── 03-bridges.hpp  # 桥与割点（Tarjan）
│   │   ├── 04-euler-trail.hpp  # 欧拉路（Hierholzer）
│   │   ├── 05-two-sat.hpp  # 2-SAT
│   │   ├── 06-min-cycle.hpp  # 最小环
│   │   ├── 07-block-cut-tree.hpp  # 圆方树
│   │   ├── 08-dominator-tree.hpp  # 支配树
│   │   ├── 09-bcc.hpp  # 双连通分量
│   │   ├── 10-connectivity.hpp  # 点 / 边连通度
│   │   ├── 11-ring-count.hpp  # 环计数
│   │   └── 12-hamilton.hpp  # 哈密顿图
│   ├── 04-matching/  # 匹配（5）
│   │   ├── 01-hopcroft-karp.hpp  # 二分图最大匹配（Hopcroft-Karp）
│   │   ├── 02-km.hpp  # 二分图最大权匹配（KM）
│   │   ├── 03-blossom.hpp  # 一般图最大匹配（带花树）
│   │   ├── 04-general-weight-matching.hpp  # 一般图最大权匹配
│   │   └── 05-stable-matching.hpp  # 稳定匹配
│   └── 05-others/  # 其他（9）
│       ├── 01-stoer-wagner.hpp  # 全局最小割（Stoer-Wagner）
│       ├── 02-prufer.hpp  # Prüfer 序列
│       ├── 03-matrix-tree.hpp  # 矩阵树定理
│       ├── 04-lgv.hpp  # LGV 引理
│       ├── 05-max-clique.hpp  # 最大团搜索
│       ├── 06-planar.hpp  # 平面图
│       ├── 07-chordal.hpp  # 弦图
│       ├── 08-coloring.hpp  # 图的着色
│       └── 09-graph-random-walk.hpp  # 图上随机游走
├── 03-graphs-notes/  # 图论常见结论及例题（0）
├── 04-network-flow/  # 网络流（6）
│   ├── 01-dinic.hpp  # 最大流（Dinic）
│   ├── 02-mcmf.hpp  # 最小费用最大流（SPFA 增广）
│   ├── 03-lower-upper-bound.hpp  # 上下界网络流
│   ├── 04-max-weight-closure.hpp  # 最大权闭合子图
│   ├── 05-min-cut-tree.hpp  # 最小割树（Gomory-Hu）
│   └── 06-min-cut.hpp  # 最小割
├── 05-number-theory/  # 数论（54）
│   ├── 01-basic/  # 基础（13）
│   │   ├── 01-sieve.hpp  # 线性筛
│   │   ├── 02-mod-pow.hpp  # 快速幂
│   │   ├── 03-exgcd.hpp  # 扩展欧几里得 / 逆元
│   │   ├── 04-crt.hpp  # 中国剩余定理
│   │   ├── 05-lucas.hpp  # Lucas 定理
│   │   ├── 06-exlucas.hpp  # 扩展 Lucas
│   │   ├── 07-euler-phi.hpp  # 欧拉函数
│   │   ├── 08-excrt.hpp  # 扩展中国剩余定理
│   │   ├── 09-linear-inverse.hpp  # 线性递推逆元
│   │   ├── 10-bezout.hpp  # 裴蜀定理 / 线性同余
│   │   ├── 11-lift-exponent.hpp  # 升幂引理（LTE）
│   │   ├── 12-factorial-mod.hpp  # 阶乘取模
│   │   └── 13-congruence-equation.hpp  # 同余方程
│   ├── 02-primes/  # 素数（3）
│   │   ├── 01-miller-rabin.hpp  # Miller-Rabin 素性测试
│   │   ├── 02-pollard-rho.hpp  # Pollard-Rho 分解质因数
│   │   └── 03-prime-counting.hpp  # 素数计数
│   ├── 03-congruence/  # 同余（10）
│   │   ├── 01-div-blocks.hpp  # 数论分块
│   │   ├── 02-bsgs.hpp  # BSGS / 离散对数
│   │   ├── 03-primitive-root.hpp  # 原根
│   │   ├── 04-quadratic-residue.hpp  # 二次剩余
│   │   ├── 05-high-residue.hpp  # 高次剩余
│   │   ├── 06-euclidean-like.hpp  # 类欧几里得
│   │   ├── 07-continued-fraction.hpp  # 连分数
│   │   ├── 08-stern-brocot.hpp  # Stern-Brocot 树
│   │   ├── 09-pell.hpp  # Pell 方程
│   │   └── 10-quadratic-field.hpp  # 二次域
│   ├── 04-combinatorics/  # 组合数学（15）
│   │   ├── 01-combination.hpp  # 组合数（模素数）
│   │   ├── 02-cantor.hpp  # 康托展开 / 逆康托
│   │   ├── 03-burnside.hpp  # Burnside / Polya
│   │   ├── 04-inclusion-exclusion.hpp  # 容斥原理
│   │   ├── 05-catalan.hpp  # 卡特兰数
│   │   ├── 06-stirling.hpp  # 斯特林数
│   │   ├── 07-derangement.hpp  # 错排
│   │   ├── 08-bell.hpp  # 贝尔数
│   │   ├── 09-partition.hpp  # 分拆数
│   │   ├── 10-vandermonde.hpp  # 范德蒙德卷积
│   │   ├── 11-polya.hpp  # Polya 计数
│   │   ├── 12-graph-enumeration.hpp  # 图论计数
│   │   ├── 13-bernoulli.hpp  # 伯努利数
│   │   ├── 14-entringer.hpp  # Entringer Number
│   │   └── 15-eulerian-number.hpp  # Eulerian Number
│   ├── 05-sieves/  # 筛法（6）
│   │   ├── 01-mobius.hpp  # 莫比乌斯反演
│   │   ├── 02-du-sieve.hpp  # 杜教筛
│   │   ├── 03-min25-sieve.hpp  # Min_25 筛
│   │   ├── 04-zhouge-sieve.hpp  # 洲阁筛
│   │   ├── 05-dirichlet-convolution.hpp  # 狄利克雷卷积
│   │   └── 06-powerful-number-sieve.hpp  # Powerful Number 筛
│   ├── 06-linear-algebra/  # 线性代数（6）
│   │   ├── 01-matrix-pow.hpp  # 矩阵快速幂
│   │   ├── 02-linear-basis.hpp  # 线性基（异或）
│   │   ├── 03-gauss.hpp  # 高斯消元（实数）
│   │   ├── 04-simplex.hpp  # 单纯形 / 线性规划
│   │   ├── 05-determinant.hpp  # 行列式
│   │   └── 06-characteristic-poly.hpp  # 特征多项式
│   └── 07-numerical/  # 数值方法（1）
│       └── 01-numerical-integration.hpp  # 数值积分
├── 05-number-theory-notes/  # 数论常见结论及例题（0）
├── 06-geometry/  # 几何（15）
│   ├── 01-point.hpp  # 点与向量
│   ├── 02-convex-hull.hpp  # 凸包（Andrew）
│   ├── 03-half-plane.hpp  # 半平面交
│   ├── 04-min-circle.hpp  # 最小圆覆盖
│   ├── 05-rotating-calipers.hpp  # 旋转卡壳
│   ├── 06-minkowski.hpp  # 闵可夫斯基和
│   ├── 07-circle.hpp  # 圆相关（交点 / 面积 / 切线）
│   ├── 08-segment-intersection.hpp  # 线段相交
│   ├── 09-scanline.hpp  # 扫描线
│   ├── 10-3d-geometry.hpp  # 三维计算几何
│   ├── 11-pick.hpp  # Pick 定理
│   ├── 12-triangulation.hpp  # 三角剖分
│   ├── 13-nearest-points.hpp  # 平面最近点对
│   ├── 14-random-incremental.hpp  # 随机增量法
│   └── 15-inversion.hpp  # 反演变换
├── 07-polynomials/  # 多项式（19）
│   ├── 01-transform/  # 变换（4）
│   │   ├── 01-ntt.hpp  # NTT / 多项式乘法
│   │   ├── 02-fft.hpp  # FFT
│   │   ├── 03-fwt.hpp  # 快速沃尔什变换（FWT）
│   │   └── 04-czt.hpp  # Chirp Z 变换
│   ├── 02-fps/  # 形式幂级数（8）
│   │   ├── 01-poly-inverse.hpp  # 多项式求逆
│   │   ├── 02-poly-log-exp.hpp  # 多项式 ln / exp
│   │   ├── 03-poly-sqrt.hpp  # 多项式开根
│   │   ├── 04-poly-divmod.hpp  # 多项式除法 / 取模
│   │   ├── 05-newton.hpp  # 多项式牛顿迭代
│   │   ├── 06-shift.hpp  # 多项式平移
│   │   ├── 07-comp-rev.hpp  # 形式幂级数复合 / 复合逆
│   │   └── 08-elementary-functions.hpp  # 多项式初等函数
│   └── 03-others/  # 其他（7）
│       ├── 01-multipoint-eval.hpp  # 多点求值
│       ├── 02-berlekamp-massey.hpp  # Berlekamp-Massey
│       ├── 03-lagrange.hpp  # 拉格朗日插值
│       ├── 04-linear-recurrence.hpp  # 线性递推
│       ├── 05-symbolic-method.hpp  # 符号化方法
│       ├── 06-lagrange-inversion.hpp  # Lagrange 反演
│       └── 07-ogf-egf.hpp  # OGF / EGF
├── 08-data-structures/  # 数据结构（44）
│   ├── 01-basic/  # 基础（10）
│   │   ├── 01-dsu.hpp  # 并查集
│   │   ├── 02-fenwick.hpp  # 树状数组
│   │   ├── 03-fenwick2d.hpp  # 二维树状数组
│   │   ├── 04-sparse-table.hpp  # ST 表（区间最值）
│   │   ├── 05-monotonic.hpp  # 单调栈 / 单调队列
│   │   ├── 06-block-decomposition.hpp  # 分块
│   │   ├── 07-pairing-heap.hpp  # 配对堆
│   │   ├── 08-block-list.hpp  # 块状链表
│   │   ├── 09-sqrt-tree.hpp  # Sqrt Tree
│   │   └── 10-block-array.hpp  # 块状数组
│   ├── 02-segment-tree/  # 线段树（8）
│   │   ├── 01-segment-tree.hpp  # 线段树（区间加、区间和）
│   │   ├── 02-li-chao.hpp  # 李超树
│   │   ├── 03-segment-tree-beats.hpp  # Segment Tree Beats
│   │   ├── 04-tree-of-trees.hpp  # 树套树
│   │   ├── 05-seg-merge-split.hpp  # 线段树合并 / 分裂
│   │   ├── 06-cat-tree.hpp  # 猫树
│   │   ├── 07-dividing-tree.hpp  # 划分树
│   │   └── 08-kinetic-tournament.hpp  # Kinetic Tournament Tree
│   ├── 03-balanced-tree/  # 平衡树（12）
│   │   ├── 01-splay.hpp  # Splay
│   │   ├── 02-treap.hpp  # Treap
│   │   ├── 03-sgt.hpp  # 替罪羊树
│   │   ├── 04-cartesian-tree.hpp  # 笛卡尔树
│   │   ├── 05-skiplist.hpp  # 跳表
│   │   ├── 06-huffman-tree.hpp  # 霍夫曼树
│   │   ├── 07-global-bst.hpp  # 全局平衡二叉树
│   │   ├── 08-wblt.hpp  # WBLT
│   │   ├── 09-avl.hpp  # AVL 树
│   │   ├── 10-sbt.hpp  # Size Balanced Tree
│   │   ├── 11-red-black.hpp  # 红黑树
│   │   └── 12-aa-tree.hpp  # AA 树
│   ├── 04-persistent/  # 可持久化（5）
│   │   ├── 01-persistent-segtree.hpp  # 可持久化线段树
│   │   ├── 02-persistent-balanced.hpp  # 可持久化平衡树
│   │   ├── 03-persistent-trie.hpp  # 可持久化 Trie
│   │   ├── 04-persistent-heap.hpp  # 可持久化可并堆
│   │   └── 05-persistent-block-array.hpp  # 可持久化块状数组
│   ├── 05-dynamic-tree/  # 动态树（6）
│   │   ├── 01-link-cut-tree.hpp  # 动态树（LCT）
│   │   ├── 02-ett.hpp  # Euler Tour Tree
│   │   ├── 03-top-tree.hpp  # Top Tree
│   │   ├── 04-divide-combine.hpp  # 析合树
│   │   ├── 05-pq-tree.hpp  # PQ 树
│   │   └── 06-finger-tree.hpp  # 手指树
│   └── 06-others/  # 其他（3）
│       ├── 01-odt.hpp  # 珂朵莉树（ODT）
│       ├── 02-leftist-tree.hpp  # 左偏树 / 可并堆
│       └── 03-kdt.hpp  # K-D Tree
├── 09-dynamic-programming/  # 动态规划（21）
│   ├── 01-basic/  # 基础（11）
│   │   ├── 01-lis.hpp  # 最长上升子序列（LIS）
│   │   ├── 02-sos.hpp  # 高维前缀和（子集和）
│   │   ├── 03-digit-dp.hpp  # 数位 DP
│   │   ├── 04-bitmask-dp.hpp  # 状压 DP
│   │   ├── 05-interval-dp.hpp  # 区间 DP
│   │   ├── 06-tree-dp.hpp  # 树形 DP
│   │   ├── 07-knapsack.hpp  # 背包
│   │   ├── 08-memo.hpp  # 记忆化搜索
│   │   ├── 09-dag-dp.hpp  # DAG 上的 DP
│   │   ├── 10-count-dp.hpp  # 计数 DP
│   │   └── 11-state-design.hpp  # 状态设计优化
│   ├── 02-optimization/  # 优化（6）
│   │   ├── 01-slope-opt.hpp  # 斜率优化
│   │   ├── 02-divide-conquer-opt.hpp  # 决策单调性 / 分治优化
│   │   ├── 03-monotone-queue-opt.hpp  # 单调队列优化
│   │   ├── 04-quadrangle-opt.hpp  # 四边形不等式优化
│   │   ├── 05-slope-trick.hpp  # Slope Trick
│   │   └── 06-wqs-binary-search.hpp  # WQS 二分
│   └── 03-special/  # 特殊 DP（4）
│       ├── 01-probability-dp.hpp  # 概率 / 期望 DP
│       ├── 02-plug-dp.hpp  # 插头 DP
│       ├── 03-dynamic-dp.hpp  # 动态 DP
│       └── 04-dp-of-dp.hpp  # DP 套 DP
├── 10-strings/  # 串（18）
│   ├── 01-matching/  # 匹配（5）
│   │   ├── 01-kmp.hpp  # KMP
│   │   ├── 02-z-function.hpp  # Z 函数（扩展 KMP）
│   │   ├── 03-manacher.hpp  # Manacher
│   │   ├── 04-ac-automaton.hpp  # AC 自动机
│   │   └── 05-bm.hpp  # Boyer-Moore 算法
│   ├── 02-suffix/  # 后缀结构（9）
│   │   ├── 01-suffix-array.hpp  # 后缀数组
│   │   ├── 02-suffix-automaton.hpp  # 后缀自动机（SAM）
│   │   ├── 03-palindromic-tree.hpp  # 回文自动机（PAM）
│   │   ├── 04-suffix-bst.hpp  # 后缀平衡树
│   │   ├── 05-general-sam.hpp  # 广义后缀自动机
│   │   ├── 06-suffix-tree.hpp  # 后缀树
│   │   ├── 07-lyndon.hpp  # Lyndon 分解
│   │   ├── 08-main-lorentz.hpp  # Main-Lorentz 算法
│   │   └── 09-sa-optimal-inplace.hpp  # 最优原地后缀排序
│   └── 03-trie-hash/  # Trie 与哈希（4）
│       ├── 01-hash.hpp  # 字符串哈希
│       ├── 02-min-representation.hpp  # 最小表示法
│       ├── 03-trie.hpp  # Trie
│       └── 04-seq-automaton.hpp  # 序列自动机
├── 11-game-theory/  # 博弈论（8）
│   ├── 01-nim.hpp  # Nim 游戏
│   ├── 02-sg-function.hpp  # SG 函数
│   ├── 03-wythoff.hpp  # 威佐夫博弈
│   ├── 04-staircase-nim.hpp  # 阶梯 Nim
│   ├── 05-anti-nim.hpp  # 反 Nim
│   ├── 06-game-dp.hpp  # 博弈 DP
│   ├── 07-zero-sum.hpp  # 零和游戏
│   └── 08-partizan.hpp  # 非公平组合游戏
├── 12-stl/  # STL 与库函数（5）
│   ├── 01-pbds.hpp  # pb_ds 平衡树
│   ├── 02-rope.hpp  # rope
│   ├── 03-bitset.hpp  # bitset 优化
│   ├── 04-gp-hash-table.hpp  # gp_hash_table
│   └── 05-custom-hash.hpp  # 自定义哈希
├── 13-optimization/  # 卡常（4）
│   ├── 01-fast-io.hpp  # 快读快写
│   ├── 02-pragma.hpp  # 编译指令
│   ├── 03-loop-unroll.hpp  # 循环展开
│   └── 04-space-optimization.hpp  # 空间优化
└── 14-misc/  # 杂类（34）
    ├── 01-search/  # 搜索（8）
    │   ├── 01-search-bidirectional-bfs.hpp  # 双向 BFS
    │   ├── 02-search-astar.hpp  # A*
    │   ├── 03-search-ida-star.hpp  # IDA*
    │   ├── 04-search-dlx.hpp  # Dancing Links
    │   ├── 05-search-alpha-beta.hpp  # Alpha-Beta 剪枝
    │   ├── 06-search-iterative-deepening.hpp  # 迭代加深搜索
    │   ├── 07-search-heuristic.hpp  # 启发式搜索
    │   └── 08-search-backtracking.hpp  # 回溯法
    ├── 02-offline/  # 离线（3）
    │   ├── 01-cdq-divide.hpp  # CDQ 分治
    │   ├── 02-offline.hpp  # 离线算法
    │   └── 03-parallel-binsearch.hpp  # 整体二分
    ├── 03-mo/  # 莫队（7）
    │   ├── 01-mo-algorithm.hpp  # 莫队
    │   ├── 02-modifiable-mo.hpp  # 带修改莫队
    │   ├── 03-mo-on-tree.hpp  # 树上莫队
    │   ├── 04-rollback-mo.hpp  # 回滚莫队
    │   ├── 05-mo-2d.hpp  # 二维莫队
    │   ├── 06-mo-secondary.hpp  # 莫队二次离线
    │   └── 07-mo-bitset.hpp  # 莫队配合 bitset
    ├── 04-random/  # 随机化（4）
    │   ├── 01-random.hpp  # 随机数
    │   ├── 02-simulated-annealing.hpp  # 模拟退火
    │   ├── 03-hill-climbing.hpp  # 爬山算法
    │   └── 04-rand-technique.hpp  # 随机化技巧
    └── 05-others/  # 其他（12）
        ├── 01-bigint.hpp  # 大整数
        ├── 02-expression-eval.hpp  # 表达式求值
        ├── 03-fraction.hpp  # 分数类
        ├── 04-frac-programming.hpp  # 分数规划
        ├── 05-hoverline.hpp  # 悬线法
        ├── 06-josephus.hpp  # 约瑟夫问题
        ├── 07-main-element.hpp  # 主元素问题
        ├── 08-garsia-wachs.hpp  # Garsia-Wachs 算法
        ├── 09-kahan.hpp  # Kahan 求和
        ├── 10-fsm.hpp  # 有限状态自动机
        ├── 11-15-puzzle.hpp  # 15-puzzle
        └── 12-job-order.hpp  # 任务规划

_template.hpp  # 新建板子的骨架模板，不排入书
```
