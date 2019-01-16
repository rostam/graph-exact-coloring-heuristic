import itertools
import numpy as np

edge_vs = {}
rows = []
cols = []
data = []
with open("unstructured_hgraph_small_P02.txt") as infile:
    for line in infile:
        if len(line.split(" ")) == 3:
            size = int(line.split(" ")[0])
            for i in range(0, size):
                edge_vs[i] = []
        elif line == "\n" or '#' in line:
            continue
        else:
            line_splitted = line.split(" ")
            edge_vs[int(line_splitted[0])].append(int(line_splitted[1][0:-1]))
    for e1 in edge_vs:
        print(len(rows),e1)
        nodes = edge_vs[e1]
        print(len(nodes))
        if len(nodes) >= 3:
            combs = itertools.combinations(nodes, 3)
            for comb in combs:
                for e2 in edge_vs:
                    if e1 > e2:
                        nodes_e2 = edge_vs[e2]
                        if set(list(comb)).issubset(set(nodes_e2)):
                            rows.append(e1)
                            cols.append(e2)

    print(rows)
    print(cols)
    # print(edge_vs)
