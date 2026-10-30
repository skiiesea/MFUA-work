#include <iostream>
#include "numerical_methods.h"
#include <motiongl/linear_animator.hpp>

int main(){ 
    Parameters p1 = {10, 5, 0.6};
    Parameters p2 = {20, 5, 0.6};
    Parameters p3 = {10, 10, 0.6};
    Parameters p4 = {10, 5, 0.9};
    Samples a = calculate(p1);
    Samples b = calculate(p2);
    Samples c = calculate(p3);
    Samples d = calculate(p4);
    saveGraph(a, b, c, d);
    LinearAnimator anim =LinearAnimatorBuilder().size(1100, 640).build();
cout << "Original experiment" << endl;
anim.run(a);
cout << "Mass x2" << endl;
anim.run(b);
cout << "Velocity x2" << endl;
anim.run(c);
cout << "Friction +50%" << endl;
anim.run(d);
return 0;
    
}