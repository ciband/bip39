#include "gtest/gtest.h"

#include "bip39_tests.h"
#include "util.h"
#include "hex.h"

#include <algorithm>
#include <cstring>
#include <set>

TEST(bip39, generate_mnemonic) { // NOLINT
	auto passphrase = BIP39::generate_mnemonic(BIP39::entropy_bits_t::_256, BIP39::language::en);
	std::set<std::string> words;
	char s[256] = {};
	strncpy(s, passphrase.to_string().c_str(), sizeof(s));
	auto pch = strtok(s, " ");
	while (pch != nullptr) {
		ASSERT_TRUE(words.insert(pch).second);
		pch = strtok(nullptr, " ");
	}

	ASSERT_EQ(24, words.size());

	passphrase = BIP39::generate_mnemonic(BIP39::entropy_bits_t::_128, BIP39::language::en);
	words.clear();
	strncpy(s, passphrase.to_string().c_str(), sizeof(s));
	pch = strtok(s, " ");
	while (pch != nullptr) {
		ASSERT_TRUE(words.insert(pch).second);
		pch = strtok(nullptr, " ");
	}

	ASSERT_EQ(12, words.size());
}

#if 0
TEST(bip39, decode_mnemonic__no_passphrase) { // NOLINT
    for (const auto& vector : mnemonic_no_passphrase) {
        const auto words = BIP39::split(vector.mnemonic, ',');
        ASSERT_TRUE(BIP39::valid_mnemonic(words, vector.language));
        const auto seed = BIP39::decode_mnemonic(words);
        ASSERT_STRCASEEQ(BIP39::BytesToHex(seed).c_str(), vector.seed.c_str());
    }
}

TEST(bip39, decode_mnemonic__trezor) { // NOLINT
    for (const auto& vector : mnemonic_trezor_vectors)
    {
        const auto words = BIP39::split(vector.mnemonic, ',');
        ASSERT_TRUE(BIP39::valid_mnemonic(words, vector.language));
        const auto seed = BIP39::decode_mnemonic(words);
        ASSERT_STRCASEEQ(BIP39::BytesToHex(seed).c_str(), vector.seed.c_str());
    }
}

TEST(bip39, decode_mnemonic__bx) { // NOLINT
    for (const auto& vector : mnemonic_bx_to_seed_vectors)
    {
        const auto words = BIP39::split(vector.mnemonic, ',');
        ASSERT_TRUE(BIP39::valid_mnemonic(words, vector.language));
        const auto seed = BIP39::decode_mnemonic(words);
        ASSERT_STRCASEEQ(BIP39::BytesToHex(seed).c_str(), vector.seed.c_str());
    }
}

#endif

TEST(bip39, validate_mnemonic__invalid) { // NOLINT
    for (const auto& mnemonic: invalid_mnemonic_tests)
    {
        const auto words = BIP39::split(mnemonic, ',');
        ASSERT_FALSE(BIP39::valid_mnemonic(words));
    }
}

TEST(bip39, create_mnemonic__trezor) { // NOLINT
    for (const mnemonic_result& vector : mnemonic_trezor_vectors)
    {
        std::vector<uint8_t> entropy = BIP39::HexToBytes(vector.entropy.c_str());
        const auto mnemonic = BIP39::create_mnemonic(entropy, vector.language);
        ASSERT_TRUE(mnemonic.size() > 0);
        ASSERT_STREQ(BIP39::join(mnemonic.begin(), mnemonic.end(), ",").c_str(), vector.mnemonic.c_str());
        ASSERT_TRUE(BIP39::valid_mnemonic(mnemonic, vector.language));
    }
}

TEST(bip39, create_mnemonic__bx) { // NOLINT
    for (const auto& vector : mnemonic_bx_new_vectors)
    {
        std::vector<uint8_t> entropy = BIP39::HexToBytes(vector.entropy.c_str());
        const auto mnemonic = BIP39::create_mnemonic(entropy, vector.language);
        ASSERT_TRUE(mnemonic.size() > 0);
        ASSERT_STREQ(BIP39::join(mnemonic.begin(), mnemonic.end(), ",").c_str(), vector.mnemonic.c_str());
        ASSERT_TRUE(BIP39::valid_mnemonic(mnemonic, vector.language));
    }
}

TEST(bip39, create_mnemonic__tiny) { // NOLINT
    std::vector<uint8_t> entropy(4, 0xa9);
    const auto mnemonic = BIP39::create_mnemonic(entropy);
    ASSERT_EQ(3u, mnemonic.size());
    ASSERT_TRUE(BIP39::valid_mnemonic(mnemonic));
}

TEST(bip39, create_mnemonic__giant) { // NOLINT
    std::vector<uint8_t> entropy(1024, 0xa9);
    const auto mnemonic = BIP39::create_mnemonic(entropy);
    ASSERT_EQ(768u, mnemonic.size());
    ASSERT_TRUE(BIP39::valid_mnemonic(mnemonic));
}

TEST(BIP39, dictionary__en_es__no_intersection) { // NOLINT
    const auto english = BIP39::get_string_table(BIP39::language::en);
    const auto spanish = BIP39::get_string_table(BIP39::language::es);
    auto intersection = 0u;
    for( auto i = 0ul; i < BIP39::NUM_BIP39_WORDS; ++i) 
    {
        const auto test = spanish[i];
        const auto it = std::find_if(english, english + BIP39::NUM_BIP39_WORDS, [test] (const char* const word) {
            return strcmp(test, word) == 0;
        });
        if (it != english + BIP39::NUM_BIP39_WORDS) {
            ++intersection;
        }
    }
    ASSERT_EQ(0u, intersection);
}

TEST(BIP39, dictionary__en_it__no_intersection) { // NOLINT
    const auto english = BIP39::get_string_table(BIP39::language::en);
    const auto italian = BIP39::get_string_table(BIP39::language::it);
    auto intersection = 0u;
    for (auto i = 0ul; i < BIP39::NUM_BIP39_WORDS; ++i)
    {
        const auto test = italian[i];
        const auto it = std::find_if(english, english + BIP39::NUM_BIP39_WORDS, [test] (const char* const word) {
            return strcmp(test, word) == 0;
        });
        if (it != english + BIP39::NUM_BIP39_WORDS) {
            ++intersection;
        }
    }
    ASSERT_EQ(0u, intersection);
}

TEST(BIP39, dictionary__fr_es__no_intersection) { // NOLINT
    const auto french = BIP39::get_string_table(BIP39::language::fr);
    const auto spanish = BIP39::get_string_table(BIP39::language::es);
    auto intersection = 0u;
    for (auto i = 0ul; i < BIP39::NUM_BIP39_WORDS; ++i)
    {
        const auto test = spanish[i];
        const auto it = std::find_if(french, french + BIP39::NUM_BIP39_WORDS, [test] (const char* const word) {
            return strcmp(test, word) == 0;
        });
        if (it != french + BIP39::NUM_BIP39_WORDS) {
            ++intersection;
        }
    }
    ASSERT_EQ(0u, intersection);
}

TEST(BIP39, dictionary__it_es__no_intersection) { // NOLINT
    const auto italian = BIP39::get_string_table(BIP39::language::it);
    const auto spanish = BIP39::get_string_table(BIP39::language::es);
    auto intersection = 0u;
    for (auto i = 0ul; i < BIP39::NUM_BIP39_WORDS; ++i)
    {
        const auto test = spanish[i];
        const auto it = std::find_if(italian, italian + BIP39::NUM_BIP39_WORDS, [test] (const char* const word) {
            return strcmp(test, word) == 0;
        });
        if (it != italian + BIP39::NUM_BIP39_WORDS) {
            ++intersection;
        }
    }
    ASSERT_EQ(0u, intersection);
}

TEST(BIP39, dictionary__fr_it__no_intersection) { // NOLINT
    const auto french = BIP39::get_string_table(BIP39::language::fr);
    const auto italian = BIP39::get_string_table(BIP39::language::it);
    auto intersection = 0u;
    for (auto i = 0ul; i < BIP39::NUM_BIP39_WORDS; ++i)
    {
        const auto test = italian[i];
        const auto it = std::find_if(french, french + BIP39::NUM_BIP39_WORDS, [test] (const char* const word) {
            return strcmp(test, word) == 0;
        });
        if (it != french + BIP39::NUM_BIP39_WORDS) {
            ++intersection;
        }
    }
    ASSERT_EQ(0u, intersection);
}

TEST(BIP39, dictionary__zh_Hans_Hant__intersection) { // NOLINT
    const auto simplified = BIP39::get_string_table(BIP39::language::zh_Hans);
    const auto traditional = BIP39::get_string_table(BIP39::language::zh_Hant);
    auto intersection = 0u;
    for (auto i = 0ul; i < BIP39::NUM_BIP39_WORDS; ++i)
    {
        const auto test = traditional[i];
        const auto it = std::find_if(simplified, simplified + BIP39::NUM_BIP39_WORDS, [test] (const char* const word) {
            return strcmp(test, word) == 0;
        });
        if (it != simplified + BIP39::NUM_BIP39_WORDS) {
            ++intersection;
        }
    }
    ASSERT_EQ(1275u, intersection);
}
