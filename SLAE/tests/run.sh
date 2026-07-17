mkdir -p build && cd build
cmake .. -DBUILD_TESTS=ON   # тесты включены по умолчанию
cmake --build .
#./SLAE_tests
ctest                      # или ./my_lib_tests напрямую