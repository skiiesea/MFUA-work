#include <iostream>
#include <bitset>

int main() {
    std::bitset<24> s1 = 0x7ff00;
    std::bitset<24> s2 = 0xff00;
    s1[0] = 1;
    std::cout << s1 << "\n";
    std::cout << s2 << "\n";
    auto s3 = s1 & s2;
    std::cout << s3.to_ulong() << "\n";
    return 0;
}