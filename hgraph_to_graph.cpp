//
// Created by rostam on 14.05.17.
//
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include <map>
#include <algorithm>
#include <omp.h>
#include <chrono>

using namespace std;

vector<string> split(string s) {
    vector<string> tokens;
    istringstream iss(s);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(tokens));
    return tokens;
}

vector<vector<int>> comb(int N, int K) {
    string bitmask(K, 1); // K leading 1's
    bitmask.resize(N, 0); // N-K trailing 0's
    vector<vector<int>> ret;

    // print integers and permute bitmask
    do {
        vector<int> one_perm;
        for (int i = 0; i < N; ++i) // [0..N-1] integers
            if (bitmask[i]) one_perm.push_back(i);
        ret.push_back(one_perm);
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
    return ret;
}

template <typename T>
bool is_subset(std::vector<T> A, std::vector<T> B)
{
    std::sort(A.begin(), A.end());
    std::sort(B.begin(), B.end());
    return std::includes(A.begin(), A.end(), B.begin(), B.end());
}

template <typename T>
bool atleast_one_subset(vector<vector<T>> As, vector<T> B) {
    for(int i=0;i < As.size();i++) {
        if(is_subset(B,As[i])) return true;
    }
    return false;
}

int main() {
    ifstream in("unstructured_hgraph_small_P02.txt");
    string line;
    getline(in, line);
    getline(in, line);
    vector<string> res = split(line);
    map<int, vector<int>> edge_nodes;
    for (int i = 0; i < stoi(res[0]); i++)
        edge_nodes[i] = vector<int>();
    getline(in, line);
    while (in) {
        getline(in, line);
        istringstream iss(line);
        int e, n;
        iss >> e >> n;
        edge_nodes[e].push_back(n);
    }
    in.close();
    vector<pair<int, int>> edges;
    omp_set_num_threads(8);
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
#pragma omp parallel for
    for (int i = 0; i < edge_nodes.size(); i++) {
        //cerr << i << " " << edges.size() << endl;
        vector<int> nodes = edge_nodes[i];
        vector<vector<int>> combs = comb((int) nodes.size(), 3);
        for (int k = i + 1; k < edge_nodes.size(); k++) {
            if (atleast_one_subset(combs, edge_nodes[k])) {
#pragma omp critical
                edges.push_back(make_pair(i, k));
            }
        }
    }
    end = std::chrono::system_clock::now();
//
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    //std::cout << "finished computation at " << std::ctime(&end_time)
    cerr << "elapsed time: " << elapsed_seconds.count() << "s\n";
    ofstream out("unstructured_graph_small_P02.txt");
    for (int i = 0; i < edges.size(); i++) {
        out << edges[i].first << " " << edges[i].second << endl;
    }
    out.flush();
    out.close();
    return 0;
}
