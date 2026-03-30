#include <chrono>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "datatypes.h"
#include "generators/GeneralizedPeterson.h"
#include "generators/Cycle.h"
#include "exact_coloring/exact_coloring.h"

// Note: c_k() is not the standard chromatic polynomial.
// It returns 0 for k < χ(G) and nonzero for k ≥ χ(G), which lets us find
// the chromatic number.  For k > χ(G) the values differ from the standard
// chromatic polynomial and are not tested here.

static int passed = 0;
static int failed = 0;

static void check(const std::string& name, bool cond) {
    if (cond) {
        std::cout << "[PASS] " << name << "\n";
        passed++;
    } else {
        std::cout << "[FAIL] " << name << "\n";
        failed++;
    }
}

// Returns the smallest k >= 1 where c_k is nonzero.
static int chromatic_number(Graph g) {
    auto F = gen_ind_set_for_g(g);
    for (int k = 1; k <= (int)num_vertices(g); k++)
        if (c_k(F, g, k) != 0) return k;
    return num_vertices(g);
}

// ── Cycle graphs ─────────────────────────────────────────────────────────────

static void test_cycles() {
    // Even cycles are bipartite → 2-colorable
    for (int n : {4, 6}) {
        Graph g = Cycle(n).generate();
        check("C_" + std::to_string(n) + " chromatic number = 2", chromatic_number(g) == 2);
    }
    // Odd cycles → 3-colorable
    for (int n : {3, 5, 7}) {
        Graph g = Cycle(n).generate();
        check("C_" + std::to_string(n) + " chromatic number = 3", chromatic_number(g) == 3);
    }

    // Zero below chromatic number, nonzero at chromatic number
    {
        Graph g3 = Cycle(3).generate();
        auto F3 = gen_ind_set_for_g(g3);
        check("c_k(C_3, 2) = 0", c_k(F3, g3, 2) == 0);
        check("c_k(C_3, 3) > 0", c_k(F3, g3, 3) > 0);
    }
    {
        Graph g4 = Cycle(4).generate();
        auto F4 = gen_ind_set_for_g(g4);
        check("c_k(C_4, 1) = 0", c_k(F4, g4, 1) == 0);
        check("c_k(C_4, 2) > 0", c_k(F4, g4, 2) > 0);
    }
    {
        Graph g5 = Cycle(5).generate();
        auto F5 = gen_ind_set_for_g(g5);
        check("c_k(C_5, 2) = 0", c_k(F5, g5, 2) == 0);
        check("c_k(C_5, 3) > 0", c_k(F5, g5, 3) > 0);
    }
    {
        Graph g6 = Cycle(6).generate();
        auto F6 = gen_ind_set_for_g(g6);
        check("c_k(C_6, 1) = 0", c_k(F6, g6, 1) == 0);
        check("c_k(C_6, 2) > 0", c_k(F6, g6, 2) > 0);
    }

    // At the chromatic number the value matches the standard chromatic
    // polynomial for C_3 (= K_3) and C_4:
    //   P(C_3, 3) = (3-1)^3 - (3-1) = 6
    //   P(C_4, 2) = (2-1)^4 + (2-1) = 2
    {
        Graph g = Cycle(3).generate();
        auto F = gen_ind_set_for_g(g);
        check("c_k(C_3, 3) = 6", c_k(F, g, 3) == 6);
    }
    {
        Graph g = Cycle(4).generate();
        auto F = gen_ind_set_for_g(g);
        check("c_k(C_4, 2) = 2", c_k(F, g, 2) == 2);
    }
}

// ── Complete graphs K_n ───────────────────────────────────────────────────────
// P(K_n, k) = k * (k-1) * ... * (k-n+1),  χ(K_n) = n
// c_k agrees with the standard polynomial exactly at k = n.

static void test_complete_graphs() {
    // K_1
    {
        Graph g;
        add_vertex(g);
        auto F = gen_ind_set_for_g(g);
        check("K_1 chromatic number = 1", chromatic_number(g) == 1);
        check("c_k(K_1, 1) = 1", c_k(F, g, 1) == 1);
    }
    // K_2: P(K_2, 2) = 2
    {
        Graph g;
        add_edge(0, 1, g);
        auto F = gen_ind_set_for_g(g);
        check("K_2 chromatic number = 2", chromatic_number(g) == 2);
        check("c_k(K_2, 1) = 0", c_k(F, g, 1) == 0);
        check("c_k(K_2, 2) = 2", c_k(F, g, 2) == 2);
    }
    // K_3: P(K_3, 3) = 6
    {
        Graph g;
        add_edge(0, 1, g); add_edge(1, 2, g); add_edge(0, 2, g);
        auto F = gen_ind_set_for_g(g);
        check("K_3 chromatic number = 3", chromatic_number(g) == 3);
        check("c_k(K_3, 2) = 0",  c_k(F, g, 2) == 0);
        check("c_k(K_3, 3) = 6",  c_k(F, g, 3) == 6);
    }
    // K_4: P(K_4, 4) = 24
    {
        Graph g;
        add_edge(0,1,g); add_edge(0,2,g); add_edge(0,3,g);
        add_edge(1,2,g); add_edge(1,3,g); add_edge(2,3,g);
        auto F = gen_ind_set_for_g(g);
        check("K_4 chromatic number = 4", chromatic_number(g) == 4);
        check("c_k(K_4, 3) = 0",   c_k(F, g, 3) == 0);
        check("c_k(K_4, 4) = 24",  c_k(F, g, 4) == 24);
    }
}

// ── Empty (edgeless) graphs ───────────────────────────────────────────────────
// χ(E_n) = 1 — every vertex gets the same color.

static void test_empty_graphs() {
    {
        Graph g;
        add_vertex(g);
        check("E_1 chromatic number = 1", chromatic_number(g) == 1);
        auto F = gen_ind_set_for_g(g);
        check("c_k(E_1, 1) = 1", c_k(F, g, 1) == 1);
    }
    {
        Graph g;
        add_vertex(g); add_vertex(g); add_vertex(g);
        check("E_3 chromatic number = 1", chromatic_number(g) == 1);
        auto F = gen_ind_set_for_g(g);
        check("c_k(E_3, 1) = 1", c_k(F, g, 1) == 1);
    }
}

// ── Petersen graph GP(5,2) ────────────────────────────────────────────────────
// The Petersen graph is 3-regular with 10 vertices, 15 edges, and χ = 3.

static void test_petersen() {
    Graph g = GeneralizedPeterson(5, 2).generate();
    check("Petersen GP(5,2) has 10 vertices", num_vertices(g) == 10);
    check("Petersen GP(5,2) has 15 edges",    num_edges(g)    == 15);
    auto F = gen_ind_set_for_g(g);
    check("Petersen GP(5,2) chromatic number = 3", chromatic_number(g) == 3);
    check("c_k(Petersen, 2) = 0", c_k(F, g, 2) == 0);
    check("c_k(Petersen, 3) > 0", c_k(F, g, 3) > 0);
}

// ── Bipartite graph K_{2,3} ───────────────────────────────────────────────────
// Any non-empty bipartite graph has χ = 2.
// Vertices {0,1} on one side, {2,3,4} on the other; all cross-edges present.

static void test_bipartite() {
    Graph g;
    for (int u : {0, 1})
        for (int v : {2, 3, 4})
            add_edge(u, v, g);
    auto F = gen_ind_set_for_g(g);
    check("K_{2,3} chromatic number = 2", chromatic_number(g) == 2);
    check("c_k(K_{2,3}, 1) = 0",  c_k(F, g, 1) == 0);
    check("c_k(K_{2,3}, 2) > 0",  c_k(F, g, 2) > 0);
}

// ── Interesting graph families ───────────────────────────────────────────────
// Graphs chosen for theoretical interest rather than size:
//   wheel graphs (first natural χ=4 examples), complete multipartite graphs
//   (Platonic solids, clean chromatic structure), and more of the GP family.

// Wheel W_n: hub vertex 0 connected to every vertex of a (n-1)-cycle on 1..n-1.
// χ(W_n) = 3 if n is odd (even rim → bipartite → 2 colors + hub needs 3rd)
// χ(W_n) = 4 if n is even (odd rim → 3 colors + hub needs 4th)
static Graph wheel(int n) {
    Graph g;
    for (int i = 1; i < n; i++) {
        add_edge(0, i, g);                   // spoke: hub to rim vertex i
        add_edge(i, (i % (n - 1)) + 1, g);  // rim: i to i+1 (wraps back to 1)
    }
    return g;
}

// Complete k-partite graph with each part of size s.
// Vertices partitioned into k groups of s: part p = { p*s, ..., p*s + s - 1 }.
// Edges between every pair of vertices in different parts.  χ = k.
static Graph complete_multipartite(int k, int s) {
    Graph g;
    int n = k * s;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (i / s != j / s)
                add_edge(i, j, g);
    return g;
}

static void test_interesting_graphs() {
    // Wheel graphs ────────────────────────────────────────────────────────────
    // Odd n → even rim C_{n-1} → χ = 3
    for (int n : {5, 7, 9}) {
        check("W_" + std::to_string(n) + " chromatic number = 3",
              chromatic_number(wheel(n)) == 3);
    }
    // Even n → odd rim C_{n-1} → χ = 4
    for (int n : {6, 8}) {
        check("W_" + std::to_string(n) + " chromatic number = 4",
              chromatic_number(wheel(n)) == 4);
    }

    // Complete multipartite graphs ─────────────────────────────────────────────
    // K_{2,2,2} = octahedron: 6 vertices, 12 edges, χ = 3
    check("K_{2,2,2} (octahedron) chromatic number = 3",
          chromatic_number(complete_multipartite(3, 2)) == 3);
    // K_{3,3,3}: 9 vertices, 27 edges, χ = 3
    check("K_{3,3,3} chromatic number = 3",
          chromatic_number(complete_multipartite(3, 3)) == 3);
    // K_{2,2,2,2}: 8 vertices, 24 edges, χ = 4
    check("K_{2,2,2,2} chromatic number = 4",
          chromatic_number(complete_multipartite(4, 2)) == 4);

    // Generalized Petersen family ──────────────────────────────────────────────
    // GP(4,1) = 3-dimensional hypercube Q_3: 8 vertices, 12 edges, bipartite → χ = 2
    {
        Graph g = GeneralizedPeterson(4, 1).generate();
        check("GP(4,1) = cube Q_3: " + std::to_string(num_vertices(g)) + "v "
              + std::to_string(num_edges(g)) + "e  chromatic number = 2",
              chromatic_number(g) == 2);
    }
    // GP(5,1) = pentagonal prism: 10 vertices, 15 edges, odd rim → χ = 3
    {
        Graph g = GeneralizedPeterson(5, 1).generate();
        check("GP(5,1) = pentagonal prism: " + std::to_string(num_vertices(g)) + "v "
              + std::to_string(num_edges(g)) + "e  chromatic number = 3",
              chromatic_number(g) == 3);
    }
    // GP(6,1) = hexagonal prism: 12 vertices, 18 edges, even rim → χ = 2
    {
        Graph g = GeneralizedPeterson(6, 1).generate();
        check("GP(6,1) = hexagonal prism: " + std::to_string(num_vertices(g)) + "v "
              + std::to_string(num_edges(g)) + "e  chromatic number = 2",
              chromatic_number(g) == 2);
    }
}

// ── SuiteSparse Matrix Collection ────────────────────────────────────────────
// Reads a Matrix Market (.mtx) file and builds an undirected graph:
//   - directed matrices are symmetrized  (edge added for A[i,j] or A[j,i])
//   - symmetric matrices are read as-is
//   - diagonal entries (self-loops) and weights are ignored
//   - the graph is returned with vertices numbered 0 .. n-1
//
// The test binary may run from either the project root or the build/
// subdirectory, so we try both relative paths.

static std::string find_data_file(const std::string& relpath) {
    if (std::ifstream(relpath))         return relpath;
    if (std::ifstream("../" + relpath)) return "../" + relpath;
    return {};
}

static Graph read_mtx_as_undirected(const std::string& path) {
    std::ifstream f(path);
    if (!f) throw std::runtime_error("Cannot open: " + path);
    // Skip banner and comment lines
    std::string line;
    while (std::getline(f, line))
        if (!line.empty() && line[0] != '%') break;
    int rows, cols, nnz;
    std::istringstream(line) >> rows >> cols >> nnz;
    int n = std::max(rows, cols);
    Graph g(n);
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '%') continue;
        std::istringstream ss(line);
        int r, c;
        if (!(ss >> r >> c)) continue;
        r--; c--;  // MTX is 1-indexed
        if (r != c && r >= 0 && c >= 0 && r < n && c < n)
            if (!edge(r, c, g).second) add_edge(r, c, g);
    }
    return g;
}

// Returns the induced subgraph on the first k vertices discovered by BFS
// starting from `start`.  Vertex indices are remapped to 0..k-1.
static Graph bfs_subgraph(const Graph& g, int start, int k) {
    std::vector<int> order;
    std::vector<bool> vis(num_vertices(g), false);
    std::queue<int> q;
    q.push(start); vis[start] = true;
    while (!q.empty() && (int)order.size() < k) {
        int v = q.front(); q.pop();
        order.push_back(v);
        for_each_neighbor(v, g, [&](int u) {
            if (!vis[u]) { vis[u] = true; q.push(u); }
        });
    }
    order.resize(std::min((int)order.size(), k));
    std::unordered_map<int,int> idx;
    for (int i = 0; i < (int)order.size(); i++) idx[order[i]] = i;
    Graph sub(order.size());
    for (int i = 0; i < (int)order.size(); i++)
        for_each_neighbor(order[i], g, [&](int u) {
            auto it = idx.find(u);
            if (it != idx.end() && it->second > i)
                add_edge(i, it->second, sub);
        });
    return sub;
}

static void test_suitesparse() {
    // vanHeukelum/cage3 — DNA electrophoresis, 3 monomers, directed → symmetrized
    // 5 vertices, 7 edges after symmetrization (self-loops removed)
    {
        auto path = find_data_file("matrices/cage3/cage3.mtx");
        if (path.empty()) { std::cout << "[SKIP] cage3.mtx not found\n"; }
        else {
            Graph g = read_mtx_as_undirected(path);
            check("vanHeukelum/cage3 (DNA, 5v, 7e): chromatic number = 3",
                  chromatic_number(g) == 3);
        }
    }
    // vanHeukelum/cage4 — DNA electrophoresis, 4 monomers, directed → symmetrized
    // 9 vertices, 20 edges after symmetrization
    {
        auto path = find_data_file("matrices/cage4/cage4.mtx");
        if (path.empty()) { std::cout << "[SKIP] cage4.mtx not found\n"; }
        else {
            Graph g = read_mtx_as_undirected(path);
            check("vanHeukelum/cage4 (DNA, 9v, 20e): chromatic number = 3",
                  chromatic_number(g) == 3);
        }
    }
    // Pajek/GD98_a — Graph Drawing contest 1998, directed, 38 vertices total.
    // We extract the 10-vertex BFS subgraph from vertex 0 (10v, 10e, χ=3).
    {
        auto path = find_data_file("matrices/GD98_a/GD98_a.mtx");
        if (path.empty()) { std::cout << "[SKIP] GD98_a.mtx not found\n"; }
        else {
            Graph full = read_mtx_as_undirected(path);
            Graph sub  = bfs_subgraph(full, 0, 10);
            check("Pajek/GD98_a BFS-10 subgraph (contest, 10v, 10e): chromatic number = 3",
                  chromatic_number(sub) == 3);
        }
    }
    // ndtest.mtx — symmetric FEM-like matrix included in the project, 42 vertices.
    // 10-vertex BFS subgraph from vertex 0 (10v, 14e, χ=2).
    {
        auto path = find_data_file("ndtest.mtx");
        if (path.empty()) { std::cout << "[SKIP] ndtest.mtx not found\n"; }
        else {
            Graph full = read_mtx_as_undirected(path);
            Graph sub  = bfs_subgraph(full, 0, 10);
            check("ndtest.mtx BFS-10 subgraph (FEM, 10v, 14e): chromatic number = 2",
                  chromatic_number(sub) == 2);
        }
    }
}

// ── Scaling tests ────────────────────────────────────────────────────────────
// Runs chromatic_number() and records wall-clock time so that performance
// degradation is visible as graph size grows.
//
// Observed scaling (Release build, single core):
//   Cycles    C_8  ~3ms  C_10  ~30ms  C_12  ~300ms  C_13  ~1.5s  C_14  ~3s
//   Bipartite K_{4,4} ~2ms  K_{6,6} ~130ms  K_{7,7} ~1s
//   Petersen  GP(6,2) ~250ms  GP(7,2) ~2.5s
//
// Limitation: c_k() accumulates int sums; intermediate terms overflow int for
// K_n with n >= 9 (the partial sums of (-1)^|V| * alpha^k exceed 2^31), so
// complete graphs K_9 and larger are not included here.

static void timed_check(const std::string& name, Graph g, int expected_chi) {
    auto t0 = std::chrono::steady_clock::now();
    int chi = chromatic_number(g);
    double ms = std::chrono::duration<double, std::milli>(
        std::chrono::steady_clock::now() - t0).count();
    std::ostringstream label;
    label << name << " chromatic number = " << expected_chi
          << "  [" << static_cast<int>(ms) << "ms]";
    check(label.str(), chi == expected_chi);
}

static void test_scaling() {
    // Cycle graphs C_8 – C_13
    // Even cycles: χ = 2 (bipartite)  Odd cycles: χ = 3
    for (int n : {8, 9, 10, 11, 12, 13}) {
        int expected = (n % 2 == 0) ? 2 : 3;
        timed_check("C_" + std::to_string(n), Cycle(n).generate(), expected);
    }

    // Complete graphs K_5 – K_8  (χ = n)
    // |F| = n+1 (only singletons), so the algorithm stays fast even though
    // the graph is dense.  K_9 and above overflow int in the polynomial sum.
    for (int n : {5, 6, 7, 8}) {
        Graph g;
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                add_edge(i, j, g);
        timed_check("K_" + std::to_string(n), g, n);
    }

    // Complete bipartite K_{m,m}  (χ = 2 for all m ≥ 1)
    // |F| = 2^m + 2^m – 1 (all subsets of each side), so it scales better
    // than cycles of the same vertex count.
    for (int m : {4, 5, 6, 7}) {
        Graph g;
        for (int i = 0; i < m; i++)
            for (int j = m; j < 2 * m; j++)
                add_edge(i, j, g);
        timed_check("K_{" + std::to_string(m) + "," + std::to_string(m) + "}",
                    g, 2);
    }

    // Generalized Petersen graphs  (χ = 3)
    timed_check("GP(6,2)",  GeneralizedPeterson(6, 2).generate(), 3);
    timed_check("GP(7,2)",  GeneralizedPeterson(7, 2).generate(), 3);
}

int main() {
    std::cout << "=== Cycle graphs ===\n";
    test_cycles();

    std::cout << "\n=== Complete graphs ===\n";
    test_complete_graphs();

    std::cout << "\n=== Empty graphs ===\n";
    test_empty_graphs();

    std::cout << "\n=== Petersen graph ===\n";
    test_petersen();

    std::cout << "\n=== Bipartite graph K_{2,3} ===\n";
    test_bipartite();

    std::cout << "\n=== Interesting graph families ===\n";
    test_interesting_graphs();

    std::cout << "\n=== SuiteSparse matrix collection ===\n";
    test_suitesparse();

    std::cout << "\n=== Scaling tests ===\n";
    test_scaling();

    std::cout << "\n" << passed << " passed, " << failed << " failed.\n";
    return failed == 0 ? 0 : 1;
}
