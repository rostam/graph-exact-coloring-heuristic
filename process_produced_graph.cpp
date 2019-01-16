//
// Created by rostami on 16.05.17.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include "datatypes.h"
#include "boost/progress.hpp"
#include "boost/random/linear_congruential.hpp"
#include "boost/graph/fruchterman_reingold.hpp"
#include "boost/graph/random_layout.hpp"
#include "boost/graph/topology.hpp"
#include "boost/graph/graphviz.hpp"

using namespace std;

class progress_cooling : public boost::linear_cooling<double>
{
    typedef boost::linear_cooling<double> inherited;

public:
    explicit progress_cooling(std::size_t iterations) : inherited(iterations)
    {
        display.reset(new boost::progress_display(iterations + 1, std::cerr));
    }

    double operator()()
    {
        ++(*display);
        return inherited::operator()();
    }

private:
    shared_ptr<boost::progress_display> display;
};

int main() {
    ifstream in("unstructured_graph_small_P02.txt");
    string line;
    int v1,v2;
    Graph g;
    while(in) {
        getline(in, line);
        stringstream ss(line);
        ss >> v1 >> v2;
        add_edge(v1,v2,g);
    }
    PositionVec position_vec(num_vertices(g));
    PositionMap position(position_vec.begin(), boost::get(boost::vertex_index, g));
    minstd_rand gen;
    double width = 500;
    double height = 500;

    int iterations = 10;

    //boost::random_graph_layout(g, position, boost::square_topology<minstd_rand>());
    boost::fruchterman_reingold_force_directed_layout(g, position,boost::square_topology<minstd_rand>());//cooling(progress_cooling(iterations)));

    ofstream out("test.dot");
    boost::write_graphviz(out, g);
}

