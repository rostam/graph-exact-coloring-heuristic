#ifndef DATATYPES_HPP
#define DATATYPES_HPP

#include <iostream>
#include <fstream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include <boost/config.hpp>
#include <boost/cstdlib.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/simple_point.hpp>
#include <boost/graph/topology.hpp>

typedef const unsigned int cuint;

using boost::edge_name;
using boost::edge_name_t;
using boost::edge_weight;
using boost::edge_weight_t;
using boost::graph_traits;
using boost::property;
using boost::property_map;
using boost::vertex_color;
using boost::vertex_color_t;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
        property<boost::vertex_color_t, int>,
        property<boost::edge_weight_t, int,
                property<boost::edge_name_t, std::string>>> Graph;

typedef boost::graph_traits<Graph>::adjacency_iterator AdjacencyIterator;
typedef boost::graph_traits<Graph>::edge_iterator E_iter;
typedef boost::graph_traits<Graph>::vertex_iterator V_iter;
typedef Graph::edge_descriptor Edge;
typedef Graph::vertex_descriptor Ver;
typedef boost::dynamic_bitset<> dynbit;
typedef boost::property_map<Graph, boost::edge_weight_t>::type edge_weight_type;

typedef std::vector<boost::square_topology<std::minstd_rand>::point> PositionVec;

template<typename Lambda>
static void for_each_v(Graph& g, Lambda func) {
    V_iter vi, vi_end;
    std::tie(vi, vi_end) = vertices(g);
    std::for_each(vi, vi_end, func);
}

template<typename Lambda>
static void for_each_e(Graph& g, Lambda func) {
    E_iter ei, ei_end;
    std::tie(ei, ei_end) = edges(g);
    std::for_each(ei, ei_end, func);
}

template<typename Lambda>
static void for_each_neighbor(const Ver& v, const Graph& g, Lambda func) {
    AdjacencyIterator vi, vi_end;
    std::tie(vi, vi_end) = boost::adjacent_vertices(v, g);
    std::for_each(vi, vi_end, func);
}

static auto ge_degree = [](std::pair<int,int> t1, std::pair<int,int> t2) {
    return t1.second >= t2.second;
};
static auto lt_degree = [](std::pair<int,int> t1, std::pair<int,int> t2) {
    return t1.second < t2.second;
};

static bool cmp_degrees(std::pair<int, std::pair<int,int>> t1, std::pair<int, std::pair<int,int>> t2) {
    if (t1.second.second < t2.second.second) return true;
    if (t1.second.second == t2.second.second && t1.second.first < t2.second.first) return true;
    return false;
}

/**
 * \brief Writes the graph in Metis format and invokes ndmetis to compute
 *        a fill-reducing vertex ordering.
 *
 * @param g       the input graph
 * @param name    base filename used for the Metis input and output files
 * @param met_ord output vector filled with the computed ordering
 */
static void getMetisOrdering(Graph& g, const std::string& name, std::vector<unsigned int>& met_ord) {
    std::ofstream of(name);
    of << num_vertices(g) << " " << num_edges(g) / 2 << "\n";
    for (int i = 0; i < (int)num_vertices(g); i++) {
        for (int j = 0; j < (int)num_vertices(g); j++) {
            if (edge(i, j, g).second)
                of << (j + 1) << " ";
        }
        of << "\n";
    }
    of.close();
    system(("ndmetis " + name).c_str());
    std::ifstream in(name + ".iperm");
    int i;
    while (in >> i)
        met_ord.push_back(i);
}

#endif
