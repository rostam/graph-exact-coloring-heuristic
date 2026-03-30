#include <iostream>
#include <set>
#include <string>

#include "datatypes.h"
#include "indset_exact_coloring/indset_exact_coloring.h"
#include "generators/Cycle.h"
#include "generators/GeneralizedPeterson.h"

// Returns the smallest k >= 1 for which c_k(G, k) != 0 — the chromatic number.
static int chromatic_number(Graph g) {
    auto F = gen_ind_set_for_g(g);
    for (int k = 1; k <= (int)num_vertices(g); k++)
        if (c_k(F, g, k) != 0) return k;
    return (int)num_vertices(g);
}

static void print_result(const std::string& name, const Graph& g) {
    std::cout << "  " << name
              << "  (V=" << num_vertices(g) << ", E=" << num_edges(g) << ")"
              << "  chi=" << chromatic_number(g) << "\n";
}

static Graph complete(int n) {
    Graph g;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            add_edge(i, j, g);
    return g;
}

static Graph wheel(int n) {
    Graph g;
    for (int i = 1; i < n; i++) {
        add_edge(0, i, g);
        add_edge(i, (i % (n - 1)) + 1, g);
    }
    return g;
}

static Graph complete_multipartite(int k, int s) {
    Graph g;
    int n = k * s;
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (i / s != j / s)
                add_edge(i, j, g);
    return g;
}

int main() {
    std::cout << "=== IndSet-Based Exact Coloring: chromatic numbers ===\n\n";

    std::cout << "Cycle graphs:\n";
    for (int n : {3, 4, 5, 6, 7})
        print_result("C_" + std::to_string(n), Cycle(n).generate());

    std::cout << "\nComplete graphs:\n";
    { Graph g; add_vertex(g); print_result("K_1", g); }
    for (int n : {2, 3, 4, 5, 6, 7, 8, 9, 10})
        print_result("K_" + std::to_string(n), complete(n));

    std::cout << "\nEdgeless graphs:\n";
    { Graph g; add_vertex(g); print_result("E_1", g); }
    { Graph g; add_vertex(g); add_vertex(g); add_vertex(g); print_result("E_3", g); }

    std::cout << "\nBipartite K_{2,3}:\n";
    { Graph g; for (int u : {0,1}) for (int v : {2,3,4}) add_edge(u,v,g);
      print_result("K_{2,3}", g); }

    std::cout << "\nWheel graphs:\n";
    for (int n : {5, 6, 7, 8, 9})
        print_result("W_" + std::to_string(n), wheel(n));

    std::cout << "\nComplete multipartite graphs:\n";
    print_result("K_{2,2,2}",   complete_multipartite(3, 2));
    print_result("K_{3,3,3}",   complete_multipartite(3, 3));
    print_result("K_{2,2,2,2}", complete_multipartite(4, 2));
    for (int k : {5, 6, 7, 8, 9})
        print_result("K_{2x" + std::to_string(k) + "}", complete_multipartite(k, 2));

    std::cout << "\nGeneralized Petersen graphs:\n";
    using P = std::pair<int,int>;
    for (auto [n, k] : std::initializer_list<P>{{4,1},{5,1},{5,2},{6,1},{6,2},{7,2}})
        print_result("GP(" + std::to_string(n) + "," + std::to_string(k) + ")",
                     GeneralizedPeterson(n, k).generate());

    return 0;
}
