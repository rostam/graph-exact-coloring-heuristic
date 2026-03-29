# Graph Exact Coloring Heuristic

A C++17 library that computes the **chromatic number** of a graph via the chromatic polynomial, using inclusion-exclusion over all independent sets. A greedy heuristic variant (`main2.cpp`) is also provided.

## Algorithm

The chromatic polynomial value at `k` is computed as:

```
c_k(G) = Σ_{V ⊆ V(G)} (-1)^|V| · α(V, F)^k
```

where `F` is the collection of all independent sets of `G` and `α(V, F)` counts the independent sets disjoint from `V`. The chromatic number χ(G) is the smallest `k` for which `c_k(G) ≠ 0`.

> **Note:** This is an exact but exponential-time algorithm; it is suitable for small graphs only.

## Dependencies

- Boost Graph Library ≥ 1.60 (`adjacency_list`, `dynamic_bitset`)
- GCC or Clang with C++17 support
- CMake ≥ 3.10
- OpenMP (optional, used by the hypergraph utility)

## Build

```bash
mkdir -p build && cd build
cmake ..
make
```

Produces two executables:

| Binary          | Source       | Description                                    |
|-----------------|--------------|------------------------------------------------|
| `exact_coloring`| `main.cpp`   | Finds chromatic number of GP(5,2) and C_3–C_6  |
| `tests`         | `tests.cpp`  | Runs the test suite and prints pass/fail counts |

## Usage

```bash
# Chromatic numbers of the Petersen graph and cycle graphs
./exact_coloring

# Run the test suite
./tests
```

Expected output from `./exact_coloring`:

```
GP(5,2): 10 vertices
Chromatic number of GP(5,2): 3
C_3: 3 vertices
Chromatic number of C_3: 3
...
```

## Project Structure

```
.
├── datatypes.h                    # Graph type aliases and traversal helpers
├── exact_coloring/
│   └── exact_coloring.h           # Chromatic polynomial via inclusion-exclusion
├── generators/
│   ├── GeneratorInterface.h       # Abstract base class for graph generators
│   ├── Cycle.h                    # Cycle graph C(n)
│   └── GeneralizedPeterson.h      # Generalized Petersen graph GP(n,k)
├── main.cpp                       # Exact chromatic number demo
├── main2.cpp                      # Greedy independent-set coloring heuristic
├── tests.cpp                      # Test suite (cycles, complete, empty, Petersen, bipartite)
├── Mtx2Graph.hpp / Mtx2Graph.cpp  # Matrix Market (.mtx) → graph conversion
├── mmio.h / mmio.cpp              # Matrix Market I/O (NIST)
├── hgraph_to_graph.cpp            # Hypergraph-to-graph conversion (OpenMP)
└── process_produced_graph.cpp     # Fruchterman-Reingold layout → GraphViz
```

### Key functions (`exact_coloring/exact_coloring.h`)

| Function               | Description                                              |
|------------------------|----------------------------------------------------------|
| `power_set(n, func)`   | Calls `func` for every subset of `{0,…,n−1}`             |
| `is_indep_set(g, S)`   | Returns true if `S` is an independent set in `g`          |
| `gen_ind_set(g, func)` | Calls `func` for every independent set of `g`             |
| `alpha(X, F)`          | Counts independent sets in `F` disjoint from `X`         |
| `c_k(F, g, k)`         | Chromatic polynomial value at `k`                        |
| `gen_ind_set_for_g(g)` | Returns all independent sets of `g` as `set<dynbit>`     |

## Documentation

```bash
doxygen docs/Doxyfile
```

CI/CD (`.github/workflows/main.yml`) builds via CMake and generates Doxygen docs on every push.

## Visualisation

The images below illustrate the heuristic idea:

![Initial Graph](coloring0.png)
*Original graph*

![Built Graph](coloring1.png)
*Subgraph used for exact coloring*

![Heuristic Coloring](coloring2.png)
*Heuristic coloring of the original graph*

![Exact Coloring](coloring3.png)
*Exact coloring of the original graph*
