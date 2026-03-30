# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

C++17 project implementing a graph coloring heuristic based on exact coloring algorithms applied to subgraphs. Uses the Boost Graph Library for graph data structures and computes chromatic polynomials via inclusion-exclusion over independent sets.

## Build

```bash
mkdir -p build && cd build
cmake ..
make
# Produces: build/exact_coloring
```

Requirements: Boost Graph Library (1.60+), GCC/Clang with C++17, OpenMP.

## Running

```bash
./exact_coloring   # runs hardcoded tests on Peterson and Cycle graphs
```

There is no formal test framework. Tests are embedded in `main.cpp`.

## Documentation

```bash
doxygen docs/Doxyfile
```

CI/CD (`.github/workflows/main.yml`) builds via CMake and generates Doxygen docs on every push.

## Architecture

### Core Algorithm (`exact_coloring/exact_coloring.h`)

The chromatic polynomial is computed using inclusion-exclusion over all independent sets:

- `power_set()` — enumerates all vertex subsets via bitmasks
- `is_indep_set()` — checks if a vertex subset is independent
- `gen_ind_set()` — collects all independent sets of a graph
- `alpha(V, F)` — counts independent sets in `F` that are subsets of `V`
- `c_k()` — computes chromatic polynomial: `Σ(-1)^|V| * α(V,F)^k`

Independent sets are stored as `boost::dynamic_bitset<>` for efficient subset checks.

### Graph Representation (`datatypes.h`)

Uses `boost::adjacency_list<vecS, vecS, undirectedS>` with vertex color properties. Key traversal helpers: `for_each_v()`, `for_each_e()`, `for_each_neighbor()` (all lambda-based templates). `datatypes_listS.h` is a variant using `listS` vertices.

### Graph Generators (`generators/`)

- `GeneratorInterface.h` — abstract base class; subclasses implement `generate()`
- `GeneralizedPeterson.h` — GP(n,k) graphs
- `Cycle.h` — C(n) cycle graphs

### Matrix Market I/O (`Mtx2Graph.hpp`, `mmio.h`)

`Mtx2Graph` reads `.mtx` files (NIST Matrix Market format) and converts them to graph representations: `MtxToBipGraph()` for bipartite graphs, `MtxToILUGraph()` for ILU graphs. `ndtest.mtx` (42×42) is the included test matrix.

### Other Utilities

- `hgraph_to_graph.cpp` — hypergraph-to-graph conversion with OpenMP parallelism
- `process_produced_graph.cpp` — applies Fruchterman-Reingold layout
- `hyper_graph_to_graph.py` — Python alternative for hypergraph conversion
- `main2.cpp` — alternative entry point exploring heuristic coloring
