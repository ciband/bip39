#include "bip39.h"

#include "en.h"

#include <cassert>

namespace Ark {
namespace Crypto {
namespace BIP39 {

namespace {

const char* const * get_string_table(language lang) {
	switch (lang) {
	case language::en: return en_table;

	default:
		assert("error unsupported language");
		return nullptr;
	};
}

}


std::string generate_mnemonic(language lang /* = language::en */, uint8_t num_words /* = 12 */) {
	assert(num_words == 12 || num_words == 24);
	std::string passphrase;
	const auto words = get_string_table(lang);
	for (auto i = 0; i < num_words; ) {
		char word[MAX_BIP39_WORD_LENGTH] = {};
		strcpy_P(word, (char*)pgm_read_ptr_far(&(words[generate_random_number(0, NUM_BIP39_WORDS - 1, true)])));
		if (passphrase.find(word) == std::string::npos) {
			passphrase += word;
			if (i != num_words - 1) {
				passphrase += ' ';
			}
			++i;
		}
	}
	return passphrase;
}

}
}
}
