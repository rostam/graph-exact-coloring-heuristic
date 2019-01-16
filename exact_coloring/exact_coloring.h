//
// Created by rostami on 28.06.17.
//

#ifndef EXACT_COLORING_EXACT_COLORING_H
#define EXACT_COLORING_EXACT_COLORING_H

#include "../datatypes.h"

template <typename Lambda>
static void power_set(int n,Lambda func) {
    for (int i = 0; i < pow(2, n); i++) {
        dynbit mybitset(n,i);
        func(mybitset);
    }
}

static bool is_indep_set(Graph g, dynbit myset) {
    for (int i = 0; i < myset.size(); i++) {
        if(myset[i]) {
            for (int j = i+1; j < myset.size(); j++) {
                if(myset[j]) {
                    if (edge(i, j, g).second) return false;
                }
            }
        }
    }
    return true;
}

template <typename Lambda>
static void gen_ind_set(Graph g,Lambda func) {
    power_set(num_vertices(g), [&](dynbit vi) {
        if (is_indep_set(g, vi)) {
            func(vi);
        }
    });
}

static int alpha(dynbit X, set<dynbit> F) {
    int cnt = 0;
    for_each(F.begin(),F.end(),[&X,&cnt](dynbit db) {
        dynbit tmp = db & X;
        if(tmp.none()) cnt++;
    });
    return cnt;
}

//F is actually the all independent set
static int c_k(set<dynbit> F, Graph g, int k) {
    int cnt = 0;
    power_set(num_vertices(g), [&](dynbit vi) {
        cnt += pow(-1.0f,vi.count())*pow(alpha(vi,F),k);
    });
    return cnt;
}

static set<dynbit> gen_ind_set_for_g(Graph& g) {
    set<dynbit> F;
    gen_ind_set(g,[&](dynbit ind1) {
        F.insert(ind1);
        gen_ind_set(g,[&](dynbit ind2){
            dynbit res = ind1 | ind2;
        });
    });
    return F;
}

#endif //EXACT_COLORING_EXACT_COLORING_H
