#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

// Print the argument vector.
template<typename T>
void PrintVector(const std::vector<T> &v) {
    std::cout << "[ ";
    for (typename std::vector<T>::const_iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "]" << std::endl;
}

// Split the argument string, tokenize it with the argument deliminator, and
// store its tokens in the argument vector.
void Split(const std::string &s, const char* delim, std::vector<std::string>& v);

// Return true if there is still progress.
bool Progress(std::vector<VIP_ENCDOUBLE> v1, std::vector<VIP_ENCDOUBLE> v2, double thresh);

#endif
