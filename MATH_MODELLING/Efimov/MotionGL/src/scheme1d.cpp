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

#include "motiongl/scheme1d.hpp"
#include <cmath>

namespace mgl
{

    Vec2 Scheme1D::dir() const {
        double a = angleDeg * 3.14159265358979323846 / 180.0;
        return {std::cos(a), std::sin(a)};
    }

    Scheme1D Scheme1D::make(int v, int w, int h) {
        Scheme1D s;
        const double W = double(w), H = double(h);
        switch (v) {
        case 0: 
            s.angleDeg = -30;
            s.origin = {0.10 * W, 0.75 * H};
            s.lengthPx = std::min(0.80 * W, 0.65 * H / 0.5);
            s.support = SupportKind::Plane;
            s.forceDir = 1;
            s.angleText = "30";
            s.angleAnchor = 1;
            s.angleHDir = -1;
            break;
        case 1: 
            s.angleDeg = 0;
            s.origin = {0.10 * W, 0.45 * H};
            s.lengthPx = 0.80 * W;
            s.support = SupportKind::Plane;
            s.forceDir = 1;
            break;
        case 2: 
            s.angleDeg = 180;
            s.origin = {0.90 * W, 0.45 * H};
            s.lengthPx = 0.80 * W;
            s.support = SupportKind::Plane;
            s.forceDir = 1;
            break;
        case 3: 
            s.angleDeg = 60;
            s.origin = {0.12 * W, 0.12 * H};
            s.lengthPx = std::min(0.70 * W, 0.75 * H / 0.866);
            s.support = SupportKind::Plane;
            s.forceDir = 1;
            s.angleText = "60";
            s.angleAnchor = 0;
            s.angleHDir = 1;
            break;
        case 4: 
            s.angleDeg = -90;
            s.origin = {0.50 * W, 0.88 * H};
            s.lengthPx = 0.72 * H;
            s.support = SupportKind::Rod;
            s.forceDir = 1;
            break;
        case 5: 
            s.angleDeg = 90;
            s.origin = {0.50 * W, 0.12 * H};
            s.lengthPx = 0.72 * H;
            s.support = SupportKind::Rod;
            s.forceDir = 1;
            break;
        case 6: 
            s.angleDeg = 135;
            s.origin = {0.88 * W, 0.15 * H};
            s.lengthPx = std::min(0.70 * W, 0.70 * H / 0.707);
            s.support = SupportKind::Plane;
            s.forceDir = -1;
            s.angleText = "45";
            s.angleAnchor = 0;
            s.angleHDir = -1;
            break;
        case 7: 
            s.angleDeg = 180;
            s.origin = {0.90 * W, 0.50 * H};
            s.lengthPx = 0.80 * W;
            s.support = SupportKind::Plane;
            s.forceDir = 1;
            break;
        case 8: 
            s.angleDeg = 0;
            s.origin = {0.12 * W, 0.50 * H};
            s.lengthPx = 0.80 * W;
            s.support = SupportKind::Rod;
            s.forceDir = 1;
            break;
        case 9: 
            s.angleDeg = 210;
            s.origin = {0.88 * W, 0.80 * H};
            s.lengthPx = std::min(0.75 * W, 0.70 * H / 0.5);
            s.support = SupportKind::Plane;
            s.forceDir = -1;
            s.angleText = "30";
            s.angleAnchor = 1;
            s.angleHDir = 1;
            break;
        default: 
            s.angleDeg = 0;
            s.origin = {0.10 * W, 0.45 * H};
            s.lengthPx = 0.80 * W;
            s.support = SupportKind::Plane;
            s.forceDir = 1;
            break;
        }
        return s;
    }

} // namespace mgl