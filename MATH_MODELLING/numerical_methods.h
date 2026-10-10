using Func = double(*)(double);

double newton(Func f, Func deriv, double a, double b, double eps=1e-6);
double my_sqrt(double);
double bisection(Func f, double a, double b, double eps=1e-6);
double chordes(Func f, double a, double b, double eps=1e-6);
double secants(Func f, double a, double b, double eps=1e-6);
