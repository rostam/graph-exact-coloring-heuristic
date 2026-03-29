#include <iostream>
#include <string>
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
static int chromatic_number(Graph& g) {
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

    std::cout << "\n" << passed << " passed, " << failed << " failed.\n";
    return failed == 0 ? 0 : 1;
}
