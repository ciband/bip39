#include "gtest/gtest.h"

#include "bip39.h"

#include <cstring>
#include <set>

TEST(crypto_bip39, generate_mnemonic) {
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

TEST(crypto_bip39, validate_mnemonic__invalid) {
    /*
    for (const auto& mnemonic: invalid_mnemonic_tests)
    {
        const auto words = split(mnemonic, ",");
        BOOST_REQUIRE(!validate_mnemonic(words));
    }
    */
}

TEST(crypto_bip39, generate_mnemonic__tiny) {
    std::vector<uint8_t> entropy(4, 0xa9);
    const auto mnemonic = BIP39::generate_mnemonic(entropy);
    ASSERT_EQ(3u, mnemonic.size());
    ASSERT_TRUE(BIP39::valid_mnemonic(mnemonic));
}

TEST(crypto_bip39, generate_mnemonic__giant) {
    std::vector<uint8_t> entropy(1024, 0xa9);
    const auto mnemonic = BIP39::generate_mnemonic(entropy);
    ASSERT_EQ(768u, mnemonic.size());
    ASSERT_TRUE(BIP39::valid_mnemonic(mnemonic));
}
