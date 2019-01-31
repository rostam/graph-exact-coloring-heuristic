#include <iostream>
#include <set>
#include <algorithm>
#include "datatypes.h"
#include "Mtx2Graph.hpp"
#include "generators/GeneralizedPeterson.h"

using namespace std;

#include "exact_coloring/exact_coloring.h"
#include "generators/GeneralizedPeterson.h"

int main(void) {
    Graph g = GeneralizedPeterson(5,2).generate();

//    add_edge(0,1,g);
//    add_edge(1,2,g);
//    add_edge(2,0,g);
//    add_edge(0,3,g);
//    add_edge(1,3,g);
//    add_edge(2,3,g);
//    add_edge(3,4,g);
//    add_edge(4,5,g);
//    add_edge(4,6,g);
//    add_edge(5,7,g);
//    for(int i=7;i<10;i++) {
//        add_edge(i,i+1,g);
//    }
    //matrix_market mm("ndtest.mtx");
    //mm.MtxToBipGraph(g);

    cerr << "Number of vertices: " << num_vertices(g) << endl;

    for(int i=1;i<10;i++) {
        int res =c_k(gen_ind_set_for_g(g),g,i);
        //cerr << i << " " << res << " ";
        if(res!=0) {
            cerr << "The number of color : " << i;
            break;
        }
    }
}
