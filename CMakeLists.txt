cmake_minimum_required(VERSION 3.6)
project(exact_coloring)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 --openmp")

#set(SOURCE_FILES main.cpp generators/GeneratorInterface.h generators/GeneralizedPetersonGenerator.h exact_coloring/exact_coloring.h datatypes.h)
#set(SOURCE_FILES mmio.cpp Mtx2Graph.cpp hgraph_to_graph.cpp process_produced_graph.cpp)
#set(SOURCE_FILES mmio.cpp Mtx2Graph.cpp hgraph_to_graph.cpp process_produced_graph.cpp)
#set(SOURCE_FILES mmio.cpp Mtx2Graph.cpp process_produced_graph.cpp)
set(SOURCE_FILES main.cpp)
add_executable(exact_coloring ${SOURCE_FILES})
