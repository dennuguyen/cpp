#include <iostream>

#include "MultiMap.hpp"

int main() {
    MultiMap<int, char, int, std::string> m;
    m[3]['c'][0] = "hello";
    m[4]['a'][1] = "world";
    std::cout << m[3]['c'][0] << std::endl;
    std::cout << m[4]['a'][1] << std::endl;
}