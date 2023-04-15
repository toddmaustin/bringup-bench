#include "../config.h"
#include "pagerank.h"
#include "sparse_matrix.h"

// Initialize the graph from a file, and assign the initial value to each node.
void PageRank::InitGraph(const std::string& filename, const std::string& filefmt) {
    graph_ = new Graph();
    graph_t_ = new Graph();

    // Read both the graph and its tranpose graph.
    graph_->ReadFromFile(filename, filefmt, false);    
    graph_t_->ReadFromFile(filename, filefmt, true);

    // Set each node value to 1 / N.
    int n = graph_->NumNodes();
    for (int i = 0; i < n; ++i) {
        graph_->SetNode(i, 1.0 / (double)n);
    }
}

// Update the graph using the pull style algorithm.
std::vector<VIP_ENCDOUBLE> PageRank::PullUpdate(double d) {
    if (d < 0.0 || d > 1.0) {
        throw std::invalid_argument("d must be between 0 and 1");
    }
    int n = graph_->NumNodes();
    std::vector<VIP_ENCDOUBLE> next(n, (VIP_ENCDOUBLE)((1.0 - d) / (double)n));
    for (int i = 0; i < n; ++i) {
        // Outgoing edges of transpose graph are incoming edges of the original graph.
        std::vector<Entry<double>*> incoming = graph_t_->OutEdges(i);
        for (std::vector<Entry<double>*>::const_iterator it = incoming.begin(); it < incoming.end(); ++it) {
            int src = (*it)->J(); // source is the destination of the transpose graph
            int out_degree = graph_->OutEdges(src).size();
            next.at(i) = next.at(i) + (VIP_ENCDOUBLE)d * (graph_->Node(src) / (double)out_degree);
        }
    }
    graph_->SetNodes(next);
    graph_t_->SetNodes(next);
    return next;
}

// Update the graph using the push style algorithm.
std::vector<VIP_ENCDOUBLE> PageRank::PushUpdate(double d) {
    if (d < 0.0 || d > 1.0) {
        throw std::invalid_argument("d must be between 0 and 1");
    }
    int n = graph_->NumNodes();
    std::vector<VIP_ENCDOUBLE> next(n, (1.0 - d) / (double)n);
    for (int i = 0; i < n; ++i) {
        std::vector<Entry<double>*> outgoing = graph_->OutEdges(i);
        int out_degree = outgoing.size();
        for (std::vector<Entry<double>*>::const_iterator it = outgoing.begin(); it < outgoing.end(); ++it) {
            int dst = (*it)->J(); // to each destination, push its value.
            next.at(dst) = next.at(dst) + (VIP_ENCDOUBLE)d * (graph_->Node(i) / (double)out_degree);
        }
    }
    graph_->SetNodes(next);
    graph_t_->SetNodes(next);
    return next;
}
