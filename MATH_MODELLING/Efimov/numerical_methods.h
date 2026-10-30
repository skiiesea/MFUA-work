
#include <vector>
#include <utility>


struct Parameters
{
    double m;
    double v0;
    double f;
};

using Samples = std::vector<std::pair<double, double>>;

double force(double t, Parameters p);

double acceleration(double t, double v, Parameters p);

double rk4(double t, double v, Parameters p);

Samples calculate(Parameters p);

void saveGraph(Samples a, Samples b, Samples c, Samples d);