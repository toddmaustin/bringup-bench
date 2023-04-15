#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include "../config.h"
#include "graph.h"
#include "sparse_matrix.h"
#include "pageutil.h"

// Clear the graph to its default state.
void Graph::Clear() {
    n_nodes_ = 0;
    n_edges_ = 0;
    delete nodes_;
    delete edges_;
}

// Read and store a graph from a file specified by the argument file name and its format.
void Graph::ReadFromFile(const std::string &filename, const std::string &format, bool transpose) {
    std::ifstream f;
    std::string line;
    std::vector<std::string> tokens;
    char err_buf[64];

    if (format.compare("DIMACS") == 0) {
        int n_nodes = 0;
        int n_edges = 0;
        bool problem_read = false;

        // Scan through the file and look for the problem line.  
        f.open(filename, std::ifstream::in);              
        for (int i = 1; std::getline(f, line) && !problem_read; ++i) {
            Split(line, " ", tokens);
            if (tokens[0].compare("p") == 0) {    
                // problem line
                if (problem_read) {
                    sprintf(err_buf, "%s:%d: miltiple problem lines found", filename.c_str(), i);
                    throw std::runtime_error(err_buf);
                }
                if (tokens.size() != 4) {
                    sprintf(err_buf, "%s:%d: invalid number of tokens", filename.c_str(), i);
                    throw std::length_error(err_buf);
                }
                n_nodes = std::stoi(tokens[2]);
                n_edges = std::stoi(tokens[3]);
                problem_read = true;
            } else if (tokens[0].compare("a") == 0 || tokens[0].compare("e") == 0) {
                // edge descriptor; shouldn't appear before finding a problem line
                sprintf(err_buf, "%s:%d: edge read before the problem", filename.c_str(), i);
                throw std::logic_error(err_buf);
            }
            tokens.clear(); 
        }
        if (!problem_read) {
            sprintf(err_buf, "%s: missing the problem line", filename.c_str());
            throw std::runtime_error(err_buf);
        }

        // Temporary containers.
        std::vector<VIP_ENCDOUBLE>* new_nodes = new std::vector<VIP_ENCDOUBLE>(n_nodes, (VIP_ENCDOUBLE)0.0);
        SparseMatrix<double>* new_edges = new SparseMatrix<double>(n_nodes, n_nodes);

        // Scan the file again and construct the graph.
        f.clear();
        f.seekg(0, std::ios::beg);
        for (int i = 1; std::getline(f, line); ++i) {
            Split(line, " ", tokens);
            if (tokens[0].compare("c") == 0) {
                // comment; do nothing
            } else if (tokens[0].compare("p") == 0) {
                // problem line; do nothing
            } else if (tokens[0].compare("a") == 0 || tokens[0].compare("e") == 0) {
                // edge descriptor
                if (tokens.size() != 4 && tokens.size() != 3) {
                    sprintf(err_buf, "%s:%d: invalid number of tokens", filename.c_str(), i);
                    throw std::length_error(err_buf);
                }
                int src = std::stoi(tokens[1]) - 1;
                int dst = std::stoi(tokens[2]) - 1;
                if (transpose) {
                    src = std::stoi(tokens[2]) - 1;
                    dst = std::stoi(tokens[1]) - 1;
                }
                double weight;
                if (tokens[0].size() == 4)
                  weight = std::stod(tokens[3]);
                else
                  weight = 0;
                new_edges->Set(src, dst, weight);
            } else {
                sprintf(err_buf, "%s:%d: line starts with unknown token", filename.c_str(), i);
                throw std::invalid_argument(err_buf);
            }
            tokens.clear();
        }
        f.close();

        // No error; copy over to the graph object.
        Clear();
        n_nodes_ = n_nodes;
        n_edges_ = n_edges;
        nodes_ = new_nodes;
        edges_ = new_edges;
    } else {
        // Handle invalid file formats.
        sprintf(err_buf, "%s: invalid file format `%s`", filename.c_str(), format.c_str());
        throw std::invalid_argument(err_buf);
    }
}
