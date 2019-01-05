#include "bip39/bip39.h"

#include <iostream>

int main(int /*argc*/, char* /*argv*/[]) {
    const auto passphrase = BIP39::generate_mnemonic();

    std::cout << passphrase << std::endl;

    return 0;
}
