# run desktop builds
cmake . -DCMAKE_BUILD_TYPE=Coverage
cmake --build . --target install

# run Gtest
./test/Debug/bip39_tests
