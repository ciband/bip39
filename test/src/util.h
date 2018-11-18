#ifndef BIP39_TESTS_UTIL_H
#define BIP39_TESTS_UTIL_H

#include "bip39/word_list.h"

#include <sstream>
#include <string>

namespace BIP39 {

template <typename InputIt>
std::string join(InputIt begin, InputIt end, const std::string& separator) {
    std::string s;

    for (auto it = begin; it != end; ++it) {
        s += *it;
        if (it + 1 != end) {
            s += separator;
        }
    }
    
    return s;
}

word_list split(const std::string& s, char delimiter)
{
    word_list tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.add(token);
    }
    return tokens;
}

}

#endif
