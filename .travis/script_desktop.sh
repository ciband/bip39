# run desktop builds
cmake . -DCMAKE_BUILD_TYPE=Coverage
cmake --build .

# run Gtest
./bin/bip39_tests
