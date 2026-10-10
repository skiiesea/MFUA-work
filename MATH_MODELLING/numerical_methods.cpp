#include "numerical_methods.h"
#include <cmath>
#include <iostream>
#include <utility>

double newton(Func f, Func deriv, double a, double b, double eps) {
    double x = a;
    int counter = 0;
    while(std::abs(f(x)) >= eps) {
        x = x - (f(x) / deriv(x));
        ++counter; 
    }

    std::cout << "newton counter: " << counter << std::endl;
    return x;
}

double my_sqrt(double c) {
    // x^2 - c = 0
    thread_local double k = c;
    auto f = [](double x) {return x * x - k;};
    auto deriv = [](double x) {return 2 * x;};
    return newton(f, deriv, 1e-6, c);
}

double bisection(Func f, double a, double b, double eps) {
    double x = (a + b) / 2;
    int counter = 0;
    while(std::abs(f(x)) >= eps)
    {
        if(f(a) * f(x) < 0) b = x;
        else a = x;
        x = (a + b) / 2;
        ++counter;
    }
    std::cout << "bisection counter: " << counter << std::endl;
    return x;
}

double chordes(Func f, double a, double b, double eps) {
    double x = a - (b - a)/(f(b) - f(a)) * f(a);
    int counter = 0;
    while(std::abs(f(x)) >= eps)
    {
        if(f(a) * f(x) < 0) b = x;
        else a = x;
        x = (a + b) / 2;
        ++counter;
    }
    std::cout << "chordes counter: " << counter << std::endl;
    return x;
}

double secants(Func f, double a, double b, double eps){
    double x1 = (a+b)/2, x2 = x1 / 2;
    int counter = 0;
    double x = x2 - (x2 - x1)/(f(x2) - f(x1)) * f(x2);
    while(std::abs(f(x)) >= eps) {
        x2 = std::exchange(x, x - (x - x2) / (f(x) - f(x2)) * f(x));
        ++counter;
    }
    std::cout << "secants counter: " << counter << std::endl;
    return x;
}