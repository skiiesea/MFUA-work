#include <cmath>
#include <vector>
#include <iostream>

auto getLagrange(const std::vector<std::pair<double, double>> &data) {
    std::vector<double> denominators;
    for (size_t i = 0, sz = data.size(); i < sz; ++i) {
        double res = 1;
        for (size_t j = 0; j < sz; ++j)
            if (i !=j) res *= data[i].first - data[j].first;
        denominators.push_back(res);
    }
return [denominators, data](double x) {
    double res = 0;
    for (size_t i = 0, sz = data.size(); i < sz; ++i){
        double elem = 1;
        for (size_t j = 0; j < sz; j++)
        if (i != j) elem *= x - data[j].first;
        res += data[i].second * elem / denominators[i];
    } 
    return res;
};

}

int main(){
    std::vector<std::pair<double, double>> data {};
    data.reserve(100);
    for (int i = 2; i < 10; ++i)
        data.push_back({i, std::log(i)});
        auto myLagrange = getLagrange(data);
        std::cout << "True value: " << std::log(2.5) << std::endl;
        std::cout << "Interpolation value: " << myLagrange(2.5) << std::endl;
        return 0;
}
