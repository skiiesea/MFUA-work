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

#include <motiongl/tube_animator.hpp>
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <vector>

int main(int argc, char **argv) {
    using namespace mgl;
    int variant = argc > 1 ? std::atoi(argv[1]) : 0;

    const double l = 2.0;            
    const double v0 = 1.0, a1 = 2.0; 
    auto seg1 = [=](double t)
    { return v0 * t + a1 * t * t / 2; };

    const double vB = v0 + a1 * 1.0; 
    std::vector<std::pair<double, double>> seg2;
    for (double tau = 0; tau <= 4.0; tau += 0.01)
        seg2.emplace_back(tau, vB * tau - 0.75 * tau * tau);

    TubeAnimator anim = TubeAnimatorBuilder()
                            .size(1100, 640)
                            .title("Bent tube ABC, variant " + std::to_string(variant))
                            .variant(variant)
                            .l(l)
                            .timeScale(1.0)
                            .build();

    anim.run(seg1, seg2);
    return 0;
}