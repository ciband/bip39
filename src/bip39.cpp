#include "bip39.h"
#include "en.h"

#include "picosha2.h"

#include <vector>
#include <random>
#include <climits>
#include <algorithm>
#include <functional>
#include <cassert>

namespace BIP39 {

using random_bytes_engine = std::independent_bits_engine<
    std::default_random_engine, 16, uint16_t>;

namespace {

const char* const * get_string_table(language lang) {
	switch (lang) {
	case language::en: return en_table;

	default:
		assert("error unsupported language");
		return nullptr;
	};
}

uint8_t bip39_shift(size_t bit)
{
    return (1 << (BYTE_BITS - (bit % BYTE_BITS) - 1));
}

int get_word_index(const char* const * const lexicon, const std::string& word) {
    for (auto i = 0u; i < NUM_BIP39_WORDS; ++i) {
        char w[MAX_BIP39_WORD_LENGTH] = {};
        strcpy_P(w, (char*)pgm_read_ptr_far(&(lexicon[i])));
        if (strcmp(w, word.c_str()) == 0) {
            return i;
        }
    }
    return -1;
}

void append_checksum_bits(std::vector<uint8_t>& entropyBuffer) {
    const auto ENT = entropyBuffer.size() * BYTE_BITS;
    const auto CS = ENT / 32u;

    picosha2::hash256_one_by_one hasher;
    hasher.process(entropyBuffer.begin(), entropyBuffer.end());
    hasher.finish();
    std::vector<uint8_t> hash(picosha2::k_digest_size);
    hasher.get_hash_bytes(hash.begin(), hash.end());
    const auto checksum_byte_count = std::min((CS / BYTE_BITS) + 1, picosha2::k_digest_size);
    for (auto i = 0u; i < checksum_byte_count; ++i) {
        entropyBuffer.push_back(hash[i]);
    }
}

}

word_list generate_mnemonic(std::vector<uint8_t>& entropy, language lang /* = language::en */) {
    const size_t entropy_bits = (entropy.size() * BYTE_BITS);
    const size_t check_bits = (entropy_bits / ENTROPY_BIT_DIVISOR);
    const size_t total_bits = (entropy_bits + check_bits);
    const size_t word_count = (total_bits / BITS_PER_WORD);

    append_checksum_bits(entropy);

    size_t bit = 0;
    const auto lexicon = get_string_table(lang);
    word_list words;
    for (size_t i = 0; i < word_count; i++)
    {
        size_t position = 0;
        for (size_t loop = 0; loop < BITS_PER_WORD; loop++)
        {
            bit = (i * BITS_PER_WORD + loop);
            position <<= 1;

            const auto byte = bit / BYTE_BITS;

            if ((entropy[byte] & bip39_shift(bit)) > 0)
                position++;
        }

        assert(position < DICTIONARY_SIZE);
        char word[MAX_BIP39_WORD_LENGTH] = {};
        strcpy_P(word, (char*)pgm_read_ptr_far(&(lexicon[position])));
        words.add(word);
    }
    return words;
}

word_list generate_mnemonic(entropy_bits_t entropy /* = entropy_bits::_128 */, language lang /* = language::en */) {
    const size_t entropy_bits = static_cast<entropy_bits_int_type>(entropy);
    assert(entropy_bits % (MNEMONIC_SEED_MULTIPLE * BYTE_BITS) == 0);

    const size_t check_bits = (entropy_bits / ENTROPY_BIT_DIVISOR);
    const size_t total_bits = (entropy_bits + check_bits);
    const size_t word_count = (total_bits / BITS_PER_WORD);

    assert((total_bits % BITS_PER_WORD) == 0);
    assert((word_count % MNEMONIC_WORD_MULTIPLE) == 0);

    random_bytes_engine rbe;
    std::vector<uint8_t> data(entropy_bits / 8);
    std::generate(begin(data), end(data), [&rbe]() { return static_cast<uint8_t>(std::ref(rbe)()); });
    return generate_mnemonic(data, lang);
}

void mnemonicToSeedHex(const word_list& mnemonic, const std::string& password /* = ""*/) {

}

bool valid_mnemonic(const word_list& words, language lang /* = language::en */) {
    const auto word_count = words.size();
    if ((word_count % MNEMONIC_WORD_MULTIPLE) != 0) {
        return false;
    }

    const auto total_bits = BITS_PER_WORD * word_count;
    const auto check_bits = total_bits / (ENTROPY_BIT_DIVISOR + 1);
    const auto entropy_bits = total_bits - check_bits;

    assert((entropy_bits % BYTE_BITS) == 0);

    size_t bit = 0;
    std::vector<uint8_t> data((total_bits + BYTE_BITS - 1) / BYTE_BITS, 0);
    const auto lexicon = get_string_table(lang);

    for (const auto& word : words)
    {
        const auto position = get_word_index(lexicon, word);
        if (position == -1)
            return false;

        for (size_t loop = 0; loop < BITS_PER_WORD; loop++, bit++)
        {
            if (position & (1 << (BITS_PER_WORD - loop - 1)))
            {
                const auto byte = bit / BYTE_BITS;
                data[byte] |= bip39_shift(bit);
            }
        }
    }

    data.resize(entropy_bits / BYTE_BITS);
    const auto mnemonic = generate_mnemonic(data, lang);
    return std::equal(mnemonic.begin(), mnemonic.end(), words.begin());
}

}
