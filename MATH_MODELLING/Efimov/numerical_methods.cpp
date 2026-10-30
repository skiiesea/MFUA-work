#include "numerical_methods.h"
#include <cmath>
#include <fstream>
#include <iostream>

const double g = 9.8;
const double angle = 3.14 / 4.0;
const double h = 0.01;
const double T = 20.0;

double force(double t, Parameters p) {
    return -sin(2*t) - t - p.m * g * sin(angle);
}

double acceleration(double t, double v, Parameters p) {
    double F = force(t, p);
    double N = p.m * g * cos(angle);
    if(v > 0) return (F - p.f * N) / p.m;
    if(v < 0) return (F + p.f * N) / p.m;
    if(abs(F) <= p.f * N) return 0;
    if(F > 0) return (F - p.f * N) / p.m;
    return (F + p.f * N) / p.m;
}

double rk4(double t, double v, Parameters p) {
    double k1 = acceleration(t, v, p);
    double k2 = acceleration(t + h / 2, v + h * k1 / 2, p);
    double k3 = acceleration(t + h / 2, v + h * k2 / 2, p);
    double k4 = acceleration(t + h, v + h * k3, p);
    return (v + h * (k1 + 2 * k2 + 2 * k3 + k4) / 6);
}

Samples calculate(Parameters p){
    Samples result;
    double x = 0;
    double v = p.v0;
    double t = 0;
    result.push_back({t, x});
    while(t < T) {
        double oldV = v;
        double newV = rk4(t, v, p);
        if(oldV * newV < 0) newV = 0;
        x += v * h + (newV - v) * h / 2;
        v = newV;
        t += h;
        result.push_back({t, x});
    }
    return result;
}

void saveGraph(Samples a, Samples b, Samples c, Samples d) {
    std::ofstream file("result.csv");
    file << "t, original, mass, speed, friction\n";
    for(int i = 0; i < a.size(); ++i){
        file << a[i].first << "," << a[i].second << "," << b[i].second << "," << c[i].second << "," << d[i].second << "\n";
    }
    file.close();
    std::cout << "Result save\n";
}