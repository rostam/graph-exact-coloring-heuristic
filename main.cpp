#include <iostream>
#include <set>
#include <algorithm>
#include "datatypes.h"
#include "Mtx2Graph.hpp"
#include "generators/GeneralizedPeterson.h"
#include "generators/Cycle.h"
#include "exact_coloring/exact_coloring.h"

using namespace std;

int main() {
    Graph g = GeneralizedPeterson(5,2).generate();
    cerr << "Number of vertices: " << num_vertices(g) << endl;

    for(int i=2;i<10;i++) {
        int res =c_k(gen_ind_set_for_g(g),g,i);
        if(res!=0) {
            cerr << "The number of color for Generalized Peterson (5,2): " << i << endl;
            break;
        }
    }

    for(int ii=3;ii <= 6;ii++) {
        g = Cycle(ii).generate();
        cerr << "Number of vertices: " << num_vertices(g) << endl;

        for (int i = 2; i < 10; i++) {
            int res = c_k(gen_ind_set_for_g(g), g, i);
            if (res != 0) {
                cerr << "The number of color for cycle " << i << ":" << endl;
                break;
            }
        }
    }
}
