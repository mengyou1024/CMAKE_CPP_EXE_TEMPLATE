#include "main.hpp"
#include <fmt/core.h>

#if __has_include(<add.hpp>)
    #include <add.hpp>
#else
auto Add(auto lsh, auto rsh) {
    return lsh + rsh;
}
#endif

int main() {
    fmt::print(HELLO);
    fmt::print("The sum of 3 and 4 is {}\n", Add(3, 4));
    return 0;
}