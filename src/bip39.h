#ifndef BIP39_BIP39_H
#define BIP39_BIP39_H

#include "word_list.h"

#include <cstdint>
#include <string>
#include <map>

namespace BIP39 {

const uint8_t MAX_BIP39_WORD_LENGTH = 8 + 1;
const size_t NUM_BIP39_WORDS = 2048;
const size_t BYTE_BITS = 8;
const size_t ENTROPY_BIT_DIVISOR = 32;
const size_t BITS_PER_WORD = 11;
const size_t MNEMONIC_SEED_MULTIPLE = 4;
const size_t MNEMONIC_WORD_MULTIPLE = 3;
const size_t DICTIONARY_SIZE = 2048;

enum class entropy_bits_t : size_t {
    _128 = 128,
    _160 = 160,
    _192 = 192,
    _224 = 224,
    _256 = 256
};

typedef std::underlying_type<entropy_bits_t>::type entropy_bits_int_type;

const std::map<entropy_bits_t, size_t> CHECKSUM_BITS = {
    { entropy_bits_t::_128, 4 },
    { entropy_bits_t::_160, 5 },
    { entropy_bits_t::_192, 6 },
    { entropy_bits_t::_224, 7 },
    { entropy_bits_t::_256, 8 }
};

enum class language : uint8_t {
	en
};

word_list generate_mnemonic(entropy_bits_t entropy = entropy_bits_t::_128, language lang = language::en);
word_list create_mnemonic(std::vector<uint8_t>& entropy, language lang = language::en);

void mnemonicToSeedHex(const word_list& mnemonic, const std::string& password = "");

bool valid_mnemonic(const word_list& mnemonic, language lang = language::en);

}

#endif
