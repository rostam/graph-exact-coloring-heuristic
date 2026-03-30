#include <iostream>
#include <algorithm>

#include "datatypes.h"
#include "generators/Cycle.h"
#include "generators/GeneralizedPeterson.h"

using namespace std;

template<typename T>
static void print_vec(const vector<T>& v) {
    for (const T& x : v)
        cout << x << " ";
    cout << "\n";
}

// Extracts one independent set by repeatedly picking the first active vertex
// and removing it along with all its neighbors from the active set.
static vector<Ver> ind_set(Graph g) {
    vector<Ver> iset;
    vector<Ver> U;
    for_each_v(g, [&](Ver v) {
        if (boost::degree(v, g) != 0)
            U.push_back(v);
    });

    while (num_edges(g) != 0) {
        Ver v = U[0];
        iset.push_back(v);
        vector<Ver> X = {v};
        for_each_neighbor(v, g, [&](Ver n) { X.push_back(n); });
        vector<Ver> tmp;
        set_difference(U.begin(), U.end(), X.begin(), X.end(), inserter(tmp, tmp.begin()));
        for (Ver x : X) boost::clear_vertex(x, g);
        U = tmp;
    }

    return iset;
}

// IndSet-removal coloring: repeatedly extracts an independent set, assigns it the
// next color, removes those vertices' edges, and repeats until the graph is
// edge-free.
static vector<int> indset_removal_color(Graph g) {
    int cur_color = 0;
    vector<Ver> U;
    vector<int> coloring(boost::num_vertices(g), 0);
    for_each_v(g, [&](Ver v) {
        if (boost::degree(v, g) != 0)
            U.push_back(v);
    });
    while (num_edges(g) != 0) {
        vector<Ver> iset = ind_set(g);
        for (Ver v : iset) coloring[v] = cur_color;
        cur_color++;
        vector<Ver> tmp;
        set_difference(U.begin(), U.end(), iset.begin(), iset.end(), inserter(tmp, tmp.begin()));
        for (Ver x : iset) boost::clear_vertex(x, g);
        U = tmp;
    }
    return coloring;
}

int main() {
    Graph g = Cycle(5).generate();
    vector<int> coloring = indset_removal_color(g);
    print_vec(coloring);
    return 0;
}
