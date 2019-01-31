#include <iostream>
#include "datatypes.h"
#include "generators/Cycle.h"
#include "generators/GeneralizedPeterson.h"
using namespace std;

template<typename T>
void print_vec(vector<T>& v) {
    cout << endl;
    for(Ver i : v) {
        cout << i << " ";
    }
    cout << endl;
}

vector<Ver> ind_set(Graph g) {
    vector<Ver> iset;
    vector<Ver> U;
    vector<Ver> V;
    for_each_v(g, [&](Ver v) {
        if(boost::degree(v,g) != 0) {
            U.push_back(v);
            V.push_back(v);
        }
    });

    while (num_edges(g) != 0) {
        Ver v = U[0];
        iset.push_back(v);
        vector<Ver> X;
        X.push_back(v);
        for_each_neighbor(v, g, [&](Ver n) {
            X.push_back(n);
        });
        vector<Ver> tmp;
        set_difference(U.begin(),U.end(),X.begin(),X.end(), std::inserter(tmp, tmp.begin()));
        for(Ver x : X) {boost::clear_vertex(x, g);}
        U = tmp;
    };

    return iset;
}

vector<int> color(Graph g) {
    int color = 0;
    vector<Ver> U;
    vector<int> coloring(boost::num_vertices(g));
    for_each_v(g, [&](Ver v) {
        if(boost::degree(v,g) != 0) {
            U.push_back(v);
        }
    });
    while (num_edges(g) != 0) {
        vector<Ver> iset = ind_set(g);
        for (Ver v : iset) coloring[v] = color;
        color++;
        vector<Ver> tmp;
        set_difference(U.begin(),U.end(),iset.begin(),iset.end(), std::inserter(tmp, tmp.begin()));
        for(Ver x : iset) {boost::clear_vertex(x, g);}
        U = tmp;
    }
    return coloring;
}

int main() {
//    Graph g = GeneralizedPetersonGenerator().generate(5,2);
    Graph g = Cycle().generate(5);
//    vector<Ver> iset = ind_set(g);
//    print_vec(iset);
    vector<int> coloring = color(g);
    print_vec(coloring);
    return 0;
}
