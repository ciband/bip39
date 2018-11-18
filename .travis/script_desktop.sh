# run desktop builds
cmake . -DCMAKE_BUILD_TYPE=Coverage
cmake --build .

# run Gtest
./test/bip39_test
