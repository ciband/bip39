#include "gtest/gtest.h"

#include "bip39.h"

#include <cstring>
#include <set>

TEST(crypto_bip39, generate_mnemonic) {
	auto passphrase = Ark::Crypto::BIP39::generate_mnemonic(Ark::Crypto::BIP39::language::en, 24);
	std::set<std::string> words;
	char s[256] = {};
	strncpy(s, passphrase.c_str(), sizeof(s));
	auto pch = strtok(s, " ");
	while (pch != nullptr) {
		ASSERT_TRUE(words.insert(pch).second);
		pch = strtok(nullptr, " ");
	}

	ASSERT_EQ(24, words.size());

	passphrase = Ark::Crypto::BIP39::generate_mnemonic(Ark::Crypto::BIP39::language::en, 12);
	words.clear();
	strncpy(s, passphrase.c_str(), sizeof(s));
	pch = strtok(s, " ");
	while (pch != nullptr) {
		ASSERT_TRUE(words.insert(pch).second);
		pch = strtok(nullptr, " ");
	}

	ASSERT_EQ(12, words.size());
}
