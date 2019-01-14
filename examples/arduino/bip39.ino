#include "bip39/bip39.h"

#include <Arduino.h>

void setup() {
  const auto passphrase = BIP39::generate_mnemonic();

  Println(passphrase);
}

void loop() {
  Sleep(1000);
}
