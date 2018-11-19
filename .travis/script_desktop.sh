# run desktop builds
cmake . -DCMAKE_BUILD_TYPE=Coverage
cmake --build .

# install
make install

# run Gtest
./test/bip39_tests
