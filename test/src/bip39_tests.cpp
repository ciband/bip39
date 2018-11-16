#include "gtest/gtest.h"

#include "bip39.h"
#include "util.h"
#include "hex.h"

#include <cstring>
#include <set>

namespace {
struct mnemonic_result
{
    std::string entropy;
    std::string mnemonic;
    std::string passphrase;
    std::string seed;
    BIP39::language language;
};

typedef std::vector<mnemonic_result> mnemonic_result_list;
}

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
    const std::vector<std::string> invalid_mnemonic_tests
    {
        // Spelling error:
        "abandon,abandon,abandon,abandon,abandon,abandon,abandon,abandon,abandon,abandon,abandon,aboot",

        // Bad lengths:
        "one",
        "one,two",
        "abandon,abandon,abandon,abandon,abandon,abandon,abandon,abandon,abandon,abandon,abandon",

        // Bad checksum:
        "abandon,abandon,abandon,abandon,abandon,abandon,abandon,abandon,abandon,abandon,abandon,one",
    };

    
    for (const auto& mnemonic: invalid_mnemonic_tests)
    {
        const auto words = BIP39::split(mnemonic, ',');
        ASSERT_FALSE(BIP39::valid_mnemonic(words));
    }
}

TEST(crypto_bip39, create_mnemonic__bx) {
    const mnemonic_result_list mnemonic_bx_new_vectors
    {
        {
            {
                "baadf00dbaadf00d",
                "rival,hurdle,address,inspire,tenant,alone",
                "",
                "",
                BIP39::language::en
            },
            {
                "baadf00dbaadf00dbaadf00dbaadf00d",
                "rival,hurdle,address,inspire,tenant,almost,turkey,safe,asset,step,lab,boy",
                "",
                "",
                BIP39::language::en
            }/*,
            {
                "baadf00dbaadf00dbaadf00dbaadf00d",
                "previo,humilde,actuar,jarabe,tabique,ahorro,tope,pulpo,anís,señal,lavar,bahía",
                "",
                "",
                BIP39::language::es
            },
            {
                "baadf00dbaadf00dbaadf00dbaadf00d",
                "ねんかん,すずしい,あひる,せたけ,ほとんど,あんまり,めいあん,のべる,いなか,ふとる,ぜんりゃく,えいせい",
                "",
                "",
                BIP39::language::ja
            },
            {
                "baadf00dbaadf00dbaadf00dbaadf00d",
                "博,肉,地,危,惜,多,陪,荒,因,患,伊,基",
                "",
                "",
                BIP39::language::zh_Hans
            },
            {
                "baadf00dbaadf00dbaadf00dbaadf00d",
                "博,肉,地,危,惜,多,陪,荒,因,患,伊,基",
                "",
                "",
                BIP39::language::zh_Hant
            }*/
        }
    };

    for (const auto& vector : mnemonic_bx_new_vectors)
    {
        std::vector<uint8_t> entropy = BIP39::HexToBytes(vector.entropy.c_str());
        const auto mnemonic = create_mnemonic(entropy, vector.language);
        ASSERT_TRUE(mnemonic.size() > 0);
        ASSERT_EQ(BIP39::join(mnemonic.begin(), mnemonic.end(), ","), vector.mnemonic);
        ASSERT_TRUE(BIP39::valid_mnemonic(mnemonic));
    }
}

TEST(crypto_bip39, create_mnemonic__tiny) {
    std::vector<uint8_t> entropy(4, 0xa9);
    const auto mnemonic = BIP39::create_mnemonic(entropy);
    ASSERT_EQ(3u, mnemonic.size());
    ASSERT_TRUE(BIP39::valid_mnemonic(mnemonic));
}

TEST(crypto_bip39, create_mnemonic__giant) {
    std::vector<uint8_t> entropy(1024, 0xa9);
    const auto mnemonic = BIP39::create_mnemonic(entropy);
    ASSERT_EQ(768u, mnemonic.size());
    ASSERT_TRUE(BIP39::valid_mnemonic(mnemonic));
}
