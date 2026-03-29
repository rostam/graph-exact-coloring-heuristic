#include <iostream>
#include <set>

#include "datatypes.h"
#include "exact_coloring/exact_coloring.h"
#include "generators/Cycle.h"
#include "generators/GeneralizedPeterson.h"

int main() {
    // Generalized Petersen graph GP(5,2) — the Petersen graph, χ = 3
    Graph g = GeneralizedPeterson(5, 2).generate();
    std::cout << "GP(5,2): " << num_vertices(g) << " vertices\n";
    for (int k = 2; k < 10; k++) {
        if (c_k(gen_ind_set_for_g(g), g, k) != 0) {
            std::cout << "Chromatic number of GP(5,2): " << k << "\n";
            break;
        }
    }

    // Cycle graphs C_3 through C_6
    for (int n = 3; n <= 6; n++) {
        g = Cycle(n).generate();
        std::cout << "C_" << n << ": " << num_vertices(g) << " vertices\n";
        for (int k = 2; k < 10; k++) {
            if (c_k(gen_ind_set_for_g(g), g, k) != 0) {
                std::cout << "Chromatic number of C_" << n << ": " << k << "\n";
                break;
            }
        }
    }
}
