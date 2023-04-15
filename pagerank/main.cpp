// Simple implementation of the PageRank algorithm.
// Author: Jin Yeom (jinyeom@utexas.edu)
//
// This program reads a graph from a file with DIMACS format, and performs
// the PageRank algorithm to rank each node in the graph.

#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <exception>
#include <math.h>

#include "../config.h"
#include "graph.h"
#include "sparse_matrix.h"
#include "pagerank.h"
#include "pageutil.h"

#define THRESHOLD 10e-4

void Usage(char *prog) {
    std::cout << "Usage: " << prog
              << " --filename FILENAME"
              << " --fmt FORMAT"
              << " --style STYLE"
              << " --df DF (default=0.85)"
              << " --csr (default=false)"
              << std::endl;
}

int main(int argc, char **argv) {    
    std::string filename;
    std::string filefmt;
    std::string style;
    double d = 0.85;            // dampening factor (default = 0.85)
    bool export_csr = false;    // True if exporting a CSR format of the graph.

    // Parse program arguments.
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--filename") == 0) {
            filename = argv[++i];
        } else if (strcmp(argv[i], "--fmt") == 0) {
            filefmt = argv[++i];
        } else if (strcmp(argv[i], "--style") == 0) {
            style = argv[++i];
            if (style.compare("pull") != 0 && style.compare("push") != 0) {
                Usage(argv[0]);
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[i], "--df") == 0) {
            d = std::stod(argv[++i]);
        } else if (strcmp(argv[i], "--csr") == 0) {
            export_csr = true;
        } else {
            Usage(argv[0]);
            return EXIT_FAILURE;
        }
    }
    if (filename.empty() || filefmt.empty() || style.empty()) {
        Usage(argv[0]);
        return EXIT_FAILURE;
    }

    std::cout << "Graph file name: " << filename << std::endl;
    std::cout << "Graph file format: " << filefmt << std::endl;
    std::cout << "PageRank algorithm style: " << style << std::endl;
    std::cout << "Damping factor: " << d << std::endl;
    std::cout << std::endl;

    PageRank* pr = new PageRank();
    try {
        pr->InitGraph(filename, filefmt);
    } catch (std::exception &e) {
        std::cout << "\x1b[31merror\x1b[0m: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Export CSR formatted matrix for the graph.
    if (export_csr) {
        std::cout << "======== Graph (CSR) ========" << std::endl;
        Graph* graph = pr->GetGraph();
        SparseMatrix<double>* edges = graph->Edges();

        std::vector<double> v;
        std::vector<Entry<double>*> a = edges->A();
        for (std::vector<Entry<double>*>::iterator it = a.begin(); it < a.end(); ++it) {
            v.push_back((*it)->V());
        }
        PrintVector<double>(v);
        PrintVector<int>(edges->IA());
        PrintVector<int>(edges->JA());
        std::cout << std::endl;

        std::cout << "======== Transpose Graph (CSR) ========" << std::endl;
        
        Graph* grapht = pr->GetGraphT();
        SparseMatrix<double>* edgest = grapht->Edges();

        std::vector<double> vt;
        std::vector<Entry<double>*> at = edgest->A();
        for (std::vector<Entry<double>*>::iterator it = at.begin(); it < at.end(); ++it) {
            vt.push_back((*it)->V());
        }
        PrintVector<double>(vt);
        PrintVector<int>(edgest->IA());
        PrintVector<int>(edgest->JA());
        std::cout << std::endl;        
    }

    // Decide the algorithm style.
    std::vector<VIP_ENCDOUBLE> (PageRank::*update)(double) = NULL;
    if (style.compare("pull") == 0) {
        update = &PageRank::PullUpdate;
    } else {
        update = &PageRank::PushUpdate;
    }

    std::vector<VIP_ENCDOUBLE>* ranks;
    VIP_ENCDOUBLE sum;
    {
      Stopwatch s("VIP_Bench Runtime");

      std::vector<VIP_ENCDOUBLE> r1 = (*pr.*update)(d);
      std::vector<VIP_ENCDOUBLE> r2 = (*pr.*update)(d);
      while (Progress(r1, r2, THRESHOLD)) {
          r1 = (*pr.*update)(d);
          r2 = (*pr.*update)(d);
      }

      // Clean up the final ranks to sum up to 1.0.
      ranks = pr->Ranks();
      sum = 0.0;
      for (std::vector<VIP_ENCDOUBLE>::iterator it = (*ranks).begin(); it < (*ranks).end(); ++it) {
          sum = sum + *it;
      }
    }
    std::cout << "======== Ranks ========" << std::endl;    
    for (size_t i = 0; i < ranks->size(); ++i) {
        std::cout << i << ": " << VIP_DEC(ranks->at(i) / sum) << std::endl;
    }
    return EXIT_SUCCESS;
}
