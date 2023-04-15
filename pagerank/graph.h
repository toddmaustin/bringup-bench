#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <string>

#include "sparse_matrix.h"

// Graph represented with a node vector and an edge matrix.
class Graph {
public:
    int NumNodes() const { return n_nodes_; }
    int NumEdges() const { return n_edges_; }

    VIP_ENCDOUBLE Node(int i) const { return nodes_->at(i); }
    Entry<double>* Edge(int src, int dst) const { return edges_->Get(src, dst); }
    std::vector<VIP_ENCDOUBLE>* Nodes() const { return nodes_; }
    SparseMatrix<double>* Edges() const { return edges_; }
    std::vector<Entry<double>*> OutEdges(int src) { return edges_->GetRow(src); }    

    void SetNode(int i, double v) { nodes_->at(i) = v; }
    void SetEdge(int src, int dst, double v) { edges_->Set(src, dst, v); }
    void SetNodes(std::vector<VIP_ENCDOUBLE> nodes) { *nodes_ = nodes; }

    // Clear the graph to its default state.
    void Clear();

    // Read and store a graph from a file specified by the argument file name and its format.
    void ReadFromFile(const std::string& filename, const std::string& format, bool transpose); 

private:
    int n_nodes_;                   // number of nodes
    int n_edges_;                   // number of edges
    std::vector<VIP_ENCDOUBLE>* nodes_;    // node values
    SparseMatrix<double>* edges_;   // edge values
};

#endif
