#include "gtest/gtest.h"

#include "bip39/word_list.h"

TEST(word_list, default_construct) { // NOLINT
    BIP39::word_list list;
    ASSERT_STREQ("", list.to_string().c_str());
}

TEST(word_list, add_word) { // NOLINT
    BIP39::word_list list;
    list.add("test");

    ASSERT_EQ(1u, list.size());
    ASSERT_STREQ("test", list.to_string().c_str());
    ASSERT_STREQ("test", list.begin()->c_str());
}
