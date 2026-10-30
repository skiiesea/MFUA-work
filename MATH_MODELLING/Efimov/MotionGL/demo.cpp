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

#include <cmath>
#include <cstdio>
#include <motiongl/animator.hpp>

    int main()
{
    using namespace mgl;
    try
    {
        constexpr double kPi = 3.14159265358979323846;
        constexpr double g = 9.81;  
        constexpr double mu = 0.10; 
        constexpr double v0 = 25.0; 
        constexpr double alpha = 60.0 * kPi / 180.0;

        const double ax = v0 * std::cos(alpha) / mu;
        const double ay = (g + mu * v0 * std::sin(alpha)) / (mu * mu);
        const double gm = g / mu;

        auto fx = [ax](double t)
        { return ax * (1.0 - std::exp(-mu * t)); };
        auto fy = [ay, gm](double t)
        { return ay * (1.0 - std::exp(-mu * t)) - gm * t; };

        double T = 5.0;
        for (double t = 0.05; t < 60.0; t += 0.01) {
            if (fy(t) < 0.0) { T = t; break; }
        }
        {
            double lo = T - 0.01, hi = T;
            for (int i = 0; i < 50; ++i) {
                double mid = 0.5 * (lo + hi);
                if (fy(mid) > 0.0) lo = mid;
                else hi = mid;
            }
            T = 0.5 * (lo + hi);
        }

        std::printf("v0 = %.1f m/s, alpha = %.0f deg, mu = %.2f 1/s\n", v0, 60.0, mu);
        std::printf("Flight time T = %.2f s, range x(T) = %.1f m, h_max ~ %.1f m\n",
                    T, fx(T), ay * (1.0 - std::exp(-mu * 2.0)) - gm * 2.0);

        Animator animator = AnimatorBuilder()
                                .size(1100, 640)
                                .title("Projectile point fly")
                                .origin(70, 60)
                                .autoFit(true)
                                .tickStep(5)
                                .timeScale(1.0)
                                .loop(true)
                                .build();

        animator.run(fx, fy, T);
    }
    catch (const std::exception &e) {
        std::printf("Error: %s\n", e.what());
        return 1;
    }
    return 0;
}