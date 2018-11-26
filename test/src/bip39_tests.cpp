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
        ASSERT_STREQ(BIP39::join(mnemonic.begin(), mnemonic.end(), ",").c_str(), vector.mnemonic.c_str());
        ASSERT_TRUE(BIP39::valid_mnemonic(mnemonic, vector.language));
    }
}

TEST(bip39, create_mnemonic__bx) {
    for (const auto& vector : mnemonic_bx_new_vectors)
    {
        std::vector<uint8_t> entropy = BIP39::HexToBytes(vector.entropy.c_str());
        const auto mnemonic = BIP39::create_mnemonic(entropy, vector.language);
        ASSERT_TRUE(mnemonic.size() > 0);
        ASSERT_STREQ(BIP39::join(mnemonic.begin(), mnemonic.end(), ",").c_str(), vector.mnemonic.c_str());
        ASSERT_TRUE(BIP39::valid_mnemonic(mnemonic, vector.language));
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
#if 0
TEST(BIP39, create_mnemonic__japanese)
{
    for (const mnemonic_result& vector : mnemonic_japanese_vectors)
    {
        BOOST_REQUIRE(vector.entropy.size() % 2 == 0);
        data_chunk entropy;
        decode_base16(entropy, vector.entropy);
        const auto mnemonic = create_mnemonic(entropy, vector.language);
        BOOST_REQUIRE(mnemonic.size() > 0);
        BOOST_REQUIRE_EQUAL(join(mnemonic, ","), vector.mnemonic);
        BOOST_REQUIRE(validate_mnemonic(mnemonic));
    }
}

TEST(BIP39, dictionary__en_es__no_intersection)
{
    const auto& english = language::en;
    const auto& spanish = language::es;
    size_t intersection = 0;
    for (const auto es : spanish)
    {
        std::string test(es);
        const auto it = std::find(english.begin(), english.end(), test);
        if (it != std::end(english))
            intersection++;
    }

    BOOST_REQUIRE_EQUAL(intersection, 0u);
}

TEST(BIP39, dictionary__en_it__no_intersection)
{
    const auto& english = language::en;
    const auto& italian = language::it;
    size_t intersection = 0;
    for (const auto it : italian)
    {
        std::string test(it);
        const auto iter = std::find(english.begin(), english.end(), test);
        if (iter != std::end(english))
            intersection++;
    }

    BOOST_REQUIRE_EQUAL(intersection, 0u);
}

TEST(BIP39, dictionary__fr_es__no_intersection)
{
    const auto& french = language::fr;
    const auto& spanish = language::es;
    size_t intersection = 0;
    for (const auto es : spanish)
    {
        std::string test(es);
        const auto it = std::find(french.begin(), french.end(), test);
        if (it != std::end(french))
            intersection++;
    }

    BOOST_REQUIRE_EQUAL(intersection, 0u);
}

TEST(BIP39, dictionary__it_es__no_intersection)
{
    const auto& italian = language::it;
    const auto& spanish = language::es;
    size_t intersection = 0;
    for (const auto es : spanish)
    {
        std::string test(es);
        const auto it = std::find(italian.begin(), italian.end(), test);
        if (it != std::end(italian))
            intersection++;
    }

    BOOST_REQUIRE_EQUAL(intersection, 0u);
}

TEST(BIP39, dictionary__fr_it__no_intersection)
{
    const auto& french = language::fr;
    const auto& italian = language::it;
    size_t intersection = 0;
    for (const auto it : italian)
    {
        std::string test(it);
        const auto iter = std::find(french.begin(), french.end(), test);
        if (iter != std::end(french))
            intersection++;
    }

    BOOST_REQUIRE_EQUAL(intersection, 0u);
}

TEST(BIP39, dictionary__zh_Hans_Hant__intersection)
{
    const auto& simplified = language::zh_Hans;
    const auto& traditional = language::zh_Hant;
    size_t intersection = 0;
    for (const auto hant : traditional)
    {
        std::string test(hant);
        const auto it = std::find(simplified.begin(), simplified.end(), test);
        if (it != std::end(simplified))
            intersection++;
    }

    BOOST_REQUIRE_EQUAL(intersection, 1275u);
}
#endif
