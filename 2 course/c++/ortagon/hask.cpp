#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <numeric>

int main() {
    std::vector<int> v(120);
    std::ostream_iterator<int> osit(std::cout, " ");
    std::iota(v.begin(), v.end(), 0);
    std::sort(v.begin(), v.end(), [](auto lhs, auto rhs) {return false;});
    std::copy(v.begin(), v.end(), osit);
    return 0;
}