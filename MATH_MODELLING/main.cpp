#include <iostream>
#include "numerical_methods.h"

double f(double x) {return x * x + 6 * x - 7;} // x_1 = 1 and x_2 = -7

double deriv(double x) {return 2 * x + 6;}

int main() {
    int a = 0, b = 100;
    double root = newton(f, deriv, a, b);
    std::cout << "root: " << root << std::endl;
    root = bisection(f, a, b);
    std::cout << "root: " << root << std::endl;
    root = chordes(f, a, b);
    std::cout << "root: " << root << std::endl;
    root = secants(f, a, b);
    std::cout << "root: " << root << std::endl;
    return 0;
}