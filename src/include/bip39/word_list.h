#ifndef BIP39_WORD_LIST_H
#define BIP39_WORD_LIST_H

#include <vector>
#include <string>

namespace BIP39 {

class word_list {
public:
    typedef std::vector<std::string> data_type;
    typedef typename data_type::const_iterator const_iterator;
    typedef typename data_type::iterator iterator;
    typedef typename data_type::size_type size_type;

private:
    data_type _words;

public:
    word_list() = default;

    void add(std::string word) {
        _words.emplace_back(std::move(word));
    }

    std::string to_string() const {
        std::string s;
        for (auto i = 0u; i < _words.size(); ++i) {
            s += _words[i];
            if (i != _words.size() - 1) {
                s += ' ';
            }
        }
        return s;
    }

    size_type size() const { return _words.size(); }
    const_iterator cbegin() const { return _words.cbegin(); }
    const_iterator cend() const { return _words.cend(); }
    const_iterator begin() const { return _words.begin(); }
    const_iterator end() const { return _words.end(); }
    iterator begin() { return _words.begin(); }
    iterator end() { return _words.end(); }
};

inline std::ostream& operator<<(std::ostream& os, const word_list& list) {
    os << list.to_string();
    return os;
}

}

#endif
