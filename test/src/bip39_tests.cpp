#include "gtest/gtest.h"

#include "bip39_tests.h"
#include "util.h"
#include "hex.h"

#include <cstring>
#include <set>

TEST(bip39, generate_mnemonic) {
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
TEST(bip39, decode_mnemonic__no_passphrase) {
    for (const auto& vector : mnemonic_no_passphrase) {
        const auto words = BIP39::split(vector.mnemonic, ',');
        ASSERT_TRUE(BIP39::valid_mnemonic(words, vector.language));
        const auto seed = BIP39::decode_mnemonic(words);
        ASSERT_STRCASEEQ(BIP39::BytesToHex(seed).c_str(), vector.seed.c_str());
    }
}

TEST(bip39, decode_mnemonic__trezor)
{
    for (const auto& vector : mnemonic_trezor_vectors)
    {
        const auto words = BIP39::split(vector.mnemonic, ',');
        ASSERT_TRUE(BIP39::valid_mnemonic(words, vector.language));
        const auto seed = BIP39::decode_mnemonic(words);
        ASSERT_STRCASEEQ(BIP39::BytesToHex(seed).c_str(), vector.seed.c_str());
    }
}

TEST(bip39, decode_mnemonic__bx)
{
    for (const auto& vector : mnemonic_bx_to_seed_vectors)
    {
        const auto words = BIP39::split(vector.mnemonic, ',');
        ASSERT_TRUE(BIP39::valid_mnemonic(words, vector.language));
        const auto seed = BIP39::decode_mnemonic(words);
        ASSERT_STRCASEEQ(BIP39::BytesToHex(seed).c_str(), vector.seed.c_str());
    }
}

#endif

TEST(bip39, validate_mnemonic__invalid) {
    for (const auto& mnemonic: invalid_mnemonic_tests)
    {
        const auto words = BIP39::split(mnemonic, ',');
        ASSERT_FALSE(BIP39::valid_mnemonic(words));
    }
}

TEST(bip39, create_mnemonic__trezor)
{
    for (const mnemonic_result& vector : mnemonic_trezor_vectors)
    {
        std::vector<uint8_t> entropy = BIP39::HexToBytes(vector.entropy.c_str());
        const auto mnemonic = BIP39::create_mnemonic(entropy, vector.language);
        ASSERT_TRUE(mnemonic.size() > 0);
        ASSERT_EQ(BIP39::join(mnemonic.begin(), mnemonic.end(), ",").c_str(), vector.mnemonic.c_str());
        ASSERT_TRUE(BIP39::valid_mnemonic(mnemonic));
    }
}

TEST(bip39, create_mnemonic__bx) {
    for (const auto& vector : mnemonic_bx_new_vectors)
    {
        std::vector<uint8_t> entropy = BIP39::HexToBytes(vector.entropy.c_str());
        const auto mnemonic = BIP39::create_mnemonic(entropy, vector.language);
        ASSERT_TRUE(mnemonic.size() > 0);
        ASSERT_STREQ(BIP39::join(mnemonic.begin(), mnemonic.end(), ",").c_str(), vector.mnemonic.c_str());
        ASSERT_TRUE(BIP39::valid_mnemonic(mnemonic));
    }
}

TEST(bip39, create_mnemonic__tiny) {
    std::vector<uint8_t> entropy(4, 0xa9);
    const auto mnemonic = BIP39::create_mnemonic(entropy);
    ASSERT_EQ(3u, mnemonic.size());
    ASSERT_TRUE(BIP39::valid_mnemonic(mnemonic));
}

TEST(bip39, create_mnemonic__giant) {
    std::vector<uint8_t> entropy(1024, 0xa9);
    const auto mnemonic = BIP39::create_mnemonic(entropy);
    ASSERT_EQ(768u, mnemonic.size());
    ASSERT_TRUE(BIP39::valid_mnemonic(mnemonic));
}
