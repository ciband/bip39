#ifndef BIP39_BIP39_H
#define BIP39_BIP39_H

#include <cstdint>
#include <string>

namespace Ark {
namespace Crypto {
namespace BIP39 {

const uint8_t MAX_BIP39_WORD_LENGTH = 8 + 1;
const uint16_t NUM_BIP39_WORDS = 2048;

enum class language : uint8_t {
	en
};

std::string generate_mnemonic(language lang = language::en, uint8_t num_words = 12);

}
}
}

#endif
