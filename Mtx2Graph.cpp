#include "Mtx2Graph.hpp"

/**
 * \brief Constructor for making matrix from list of edges
 *
 * @param mat list of edges
 * @param m the number of rows
 * @param n the number of columns
 * @param directed is directed or not
 */
matrix_market::matrix_market(vector<pair<int,int>> mat, int m, int n,bool directed) {
    nz = mat.size();
    mm_set_matrix(&matcode);
    mm_set_sparse(&matcode);
    if(directed) mm_set_general(&matcode);
    else mm_set_symmetric(&matcode);
    mm_set_pattern(&matcode);
    /* reseve memory for matrices */
    I = (unsigned int *) malloc(nz * sizeof(unsigned int));
    J = (unsigned int *) malloc(nz * sizeof(unsigned int));
    M=m;N=n;
    int cnt = 0;
    for_each(mat.begin(),mat.end(),[&](pair<int,int> p) {
        I[cnt]=p.first;J[cnt]=p.second;
        cnt++;
    });
}
/**
 * \brief Constructor which gets a graph and make the matrix
 *
 * @param G the input graph (can be directed or undirected)
 * @param m the number of rows
 * @param n the number of cols
 * @return
 */
matrix_market::matrix_market(Graph& G, int m, int n) {
    nz = num_edges(G) + num_vertices(G);
    mm_set_matrix(&matcode);
    mm_set_sparse(&matcode);
    if(is_directed(G)) {
        mm_set_general(&matcode);
    }
    else mm_set_symmetric(&matcode);
    mm_set_pattern(&matcode);
    /* reseve memory for matrices */
    I = (unsigned int *) malloc(nz * sizeof(unsigned int));
    J = (unsigned int *) malloc(nz * sizeof(unsigned int));

    int cnt = 0;
    for(E_iter e = edges(G).first; e != edges(G).second;e++) {
        I[cnt] = source(*e,G)+1;J[cnt] = target(*e,G)+1;cnt++;
    }
    int edge_cnt = num_edges(G);
    for(int i=0;i < num_vertices(G);i++) {I[edge_cnt+i] = i+1;J[edge_cnt+i]=i+1;}
    M=m;N=n;
}

/**
 * \briefConstructor which gets a bigraph and make the matrix
 * from the edges marked by a tag
 *
 * @param G_b a bipartite graph
 * @param tag the tag related to edges
 * @param m the number of rows
 * @param n the number of cols
 * @return
 */
matrix_market::matrix_market(Graph& G_b, string tag, int m, int n, bool bipartite) {
    nz = (int) count_if(edges(G_b).first, edges(G_b).second, [&](Edge e) {
            return get(edge_name,G_b,e) == tag;
        });
    mm_set_matrix(&matcode);
    mm_set_sparse(&matcode);
    if(is_directed(G_b)) {
        mm_set_general(&matcode);
    }
    else mm_set_symmetric(&matcode);
    mm_set_pattern(&matcode);
    /* reseve memory for matrices */
    I = (unsigned int *) malloc(nz * sizeof(unsigned int));
    J = (unsigned int *) malloc(nz * sizeof(unsigned int));
    int cnt = 0;
    for_each_e(G_b,[&](Edge e) {
        if(get(edge_name,G_b,e) == tag) {
            int src = source(e, G_b);
            int tgt = target(e, G_b);
            if(bipartite) {
                if (src > m) {
                    I[cnt] = src - m + 1;
                    J[cnt] = tgt + 1;
                } else {
                    I[cnt] = tgt - m + 1;
                    J[cnt] = src + 1;
                }
            } else {
                I[cnt] = src + 1;
                J[cnt] = tgt + 1;
            }
            cnt++;
        }
    });
    M=m;N=n;
}



/**
 * \brief Write the matrix into the file with format mtx
 *
 * @param filename the name of file
 * @return true if it works correctly
 */
bool matrix_market::writeToFile(char* filename) {
    const int size =nz;
    double val[size];
    fill(val,val+size,1);
    mm_write_mtx_crd(filename, M, N, nz, (int *) I, (int *) J, val, matcode);
    return true;
}

/**
 * \brief gets the name of a mtx file and makes the matrix
 *
 * @param filename the name of the matrix file with format mtx
 * @return
 */
matrix_market::matrix_market(const char* filename) {
    int ret_code;
    FILE *file;
    int i;
    cerr << " test 1" ;
    if ((file = fopen(filename, "r")) == NULL)
        exit(1);
    cerr << " test 2" ;
    if (mm_read_banner(file, &matcode) != 0) {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }
    cerr << " test 3" ;
    /*  This is how one can screen matrix types if their application */
    /*  only supports a subset of the Matrix Market data types.      */

    if (mm_is_complex(matcode) && mm_is_matrix(matcode) &&
        mm_is_sparse(matcode)) {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    /* find out size of sparse matrix .... */

    if ((ret_code = mm_read_mtx_crd_size(file, &M, &N, &nz)) != 0)
        exit(1);

    /* reseve memory for matrices */
    I = (unsigned int *) malloc(nz * sizeof(unsigned int));
    J = (unsigned int *) malloc(nz * sizeof(unsigned int));

    if (mm_is_pattern(matcode)) {
        for (i = 0; i < nz; i++) {
            fscanf(file, "%u %u\n", &I[i], &J[i]);
            I[i]--;  /* adjust from 1-based to 0-based */
            J[i]--;
        }
    } else {
        double *val;
        val = (double *) malloc(nz * sizeof(double));
        for (i = 0; i < nz; i++) {
            fscanf(file, "%u %u %lg\n", &I[i], &J[i], &val[i]);
            I[i]--;  /* adjust from 1-based to 0-based */
            J[i]--;
        }
        free(val);
    }

    if (file != stdin) fclose(file);
}

/**
 * \brief return the bipartite graph generated from the matrix
 *
 * @param G_b the result matrix
 * @return
 */
bool matrix_market::MtxToBipGraph(Graph& G_b) {
    if (mm_is_symmetric(matcode)) {
        // add the edges to the graph object
        for (int i = 0; i < nz; ++i) {
            add_edge(I[i], M + J[i], 0, G_b);
            if (I[i] != J[i])
                add_edge(J[i], M + I[i], 0, G_b);
        }
    } else {
        // add the edges to the graph object
        for (int i = 0; i < nz; ++i) {
            add_edge(I[i], M + J[i], 0, G_b);
        }
    }

    return EXIT_SUCCESS;
}

/**
 * \brief Convert the matrix to a directed or undirected Graph
 *
 * @param G_ilu the output graph
 * @return
 */
bool matrix_market::MtxToILUGraph(Graph& G_ilu) {
    if (mm_is_symmetric(matcode)) {
        // add the edges to the graph object
        for (int i = 0; i < nz; ++i) {
            if (I[i] != J[i])
                add_edge(I[i], J[i], 0, G_ilu);
//            if (I[i] != J[i])
//                if(!edge(J[i], I[i], G_ilu).second)
//                    add_edge(J[i], I[i], 0, G_ilu);
        }
    } else {
        // add the edges to the graph object
        for (int i = 0; i < nz; ++i) {
            add_edge(I[i], J[i], 0, G_ilu);
        }
    }
    return EXIT_SUCCESS;
}

matrix_market::~matrix_market() {
  free(I);
  free(J);
}
