#ifndef PAGERANK_H
#define PAGERANK_H

#include <string>

#include "graph.h"

class PageRank {
public:
    // Initialize the graph from a file.
    void InitGraph(const std::string& filename, const std::string& format);

    Graph* GetGraph() const { return graph_; }
    Graph* GetGraphT() const { return graph_t_; }
    std::vector<VIP_ENCDOUBLE>* Ranks() { return graph_->Nodes(); }

    // Update the graph using the pull style algorithm.
    std::vector<VIP_ENCDOUBLE> PullUpdate(double d);

    // Update the graph using the push style algorithm.    
    std::vector<VIP_ENCDOUBLE> PushUpdate(double d);

private:
    Graph* graph_;      // original graph
    Graph* graph_t_;    // transpose graph
};

#endif
