// MotionGL - OpenGL animation library for classical mechanics labworks
// Copyright (C) 2026 Stanislav Furmavnin (GitHub: saintninja)
// SPDX-License-Identifier: GPL-3.0-or-later
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <motiongl/linear_animator.hpp>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

int main(int argc, char **argv) {
    using namespace mgl;
    int variant = 2;
    if (argc > 1)
        variant = std::atoi(argv[1]);

    const double m = 1.0, g = 9.81, f = 0.2, x0 = 0.2, v0 = 0.1;
    auto accel = [=](double t, double /*x*/, double v) {
        double F = 5.0 * t * t + 10.0;
        double Fr = f * m * g * (v > 0 ? 1.0 : (v < 0 ? -1.0 : 0.0));
        return (F - Fr) / m;
    };

    std::vector<std::pair<double, double>> samples;
    double t = 0, x = x0, v = v0;
    const double h = 0.01;
    samples.emplace_back(t, x);
    while (t < 20.0) {
        double k1x = v, k1v = accel(t, x, v);
        double k2x = v + .5 * h * k1v, k2v = accel(t + .5 * h, x + .5 * h * k1x, v + .5 * h * k1v);
        double k3x = v + .5 * h * k2v, k3v = accel(t + .5 * h, x + .5 * h * k2x, v + .5 * h * k2v);
        double k4x = v + h * k3v, k4v = accel(t + h, x + h * k3x, v + h * k3v);
        x += h / 6 * (k1x + 2 * k2x + 2 * k3x + k4x);
        v += h / 6 * (k1v + 2 * k2v + 2 * k3v + k4v);
        t += h;
        samples.emplace_back(t, x);
    }

    LinearAnimator anim = LinearAnimatorBuilder()
                              .size(1100, 640)
                              .title("Variant " + std::to_string(variant) + ": motion along Ox")
                              .variant(variant)
                              .timeScale(2.0) 
                              .loop(true)
                              .build();

    anim.run(samples);
#if 0
    anim.run([=](double tt) {
        return x0 + v0*tt + (10.0 - f*g)*tt*tt/2.0 + 5.0*tt*tt*tt*tt/12.0;
    }, 20.0);
#endif
    return 0;
}