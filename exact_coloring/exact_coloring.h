#ifndef EXACT_COLORING_EXACT_COLORING_H
#define EXACT_COLORING_EXACT_COLORING_H

#include <cmath>
#include <set>

#include "../datatypes.h"

// Calls func(bitset) for every subset of {0, ..., n-1}.
template <typename Lambda>
static void power_set(int n, Lambda func) {
    for (int i = 0; i < (1 << n); i++) {
        dynbit mybitset(n, i);
        func(mybitset);
    }
}

// Returns true if no two set vertices in myset are adjacent in g.
static bool is_indep_set(Graph g, dynbit myset) {
    for (int i = 0; i < (int)myset.size(); i++) {
        if (!myset[i]) continue;
        for (int j = i + 1; j < (int)myset.size(); j++) {
            if (myset[j] && edge(i, j, g).second)
                return false;
        }
    }
    return true;
}

// Calls func(bitset) for every independent set of g (including the empty set).
template <typename Lambda>
static void gen_ind_set(Graph g, Lambda func) {
    power_set(num_vertices(g), [&](dynbit vi) {
        if (is_indep_set(g, vi))
            func(vi);
    });
}

// Returns the number of independent sets in F that are disjoint from X.
static int alpha(dynbit X, std::set<dynbit> F) {
    int cnt = 0;
    for (const dynbit& db : F) {
        if ((db & X).none()) cnt++;
    }
    return cnt;
}

// Computes the chromatic polynomial value at k using inclusion-exclusion over
// all independent sets F of g:  sum_{V} (-1)^|V| * alpha(V, F)^k
// Uses long long to avoid overflow for dense graphs (e.g. K_n with n >= 9).
static long long c_k(std::set<dynbit> F, Graph g, int k) {
    long long cnt = 0;
    power_set(num_vertices(g), [&](dynbit vi) {
        long long a = alpha(vi, F);
        long long ak = 1;
        for (int i = 0; i < k; i++) ak *= a;
        cnt += (vi.count() % 2 == 0) ? ak : -ak;
    });
    return cnt;
}

// Collects all independent sets of g into a set<dynbit>.
static std::set<dynbit> gen_ind_set_for_g(Graph& g) {
    std::set<dynbit> F;
    gen_ind_set(g, [&](dynbit ind) {
        F.insert(ind);
    });
    return F;
}

#endif // EXACT_COLORING_EXACT_COLORING_H
