#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <math.h>

#include "../config.h"

// Split the argument string, tokenize it with the argument deliminator, and
// store its tokens in the argument vector.
void Split(const std::string &s, const char *delim, std::vector<std::string> &v) {
    char *dup = strdup(s.c_str());
    char *token = strtok(dup, delim);
    while (token != NULL) {
        v.push_back(std::string(token));
        token = strtok(NULL, delim);
    }
    free(dup);
}

// Return true if there is still progress.
bool Progress(std::vector<VIP_ENCDOUBLE> v1, std::vector<VIP_ENCDOUBLE> v2, double thresh) {
    for (size_t i = 0; i < v1.size(); ++i) {
        // FIXME: assuming that this info can be decrypted
        if (fabs(VIP_DEC(v2.at(i) - v1.at(i))) > thresh) {
            return true;
        }
    }
    return false;
}
