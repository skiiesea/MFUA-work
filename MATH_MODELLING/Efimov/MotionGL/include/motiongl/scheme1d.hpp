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

#pragma once
#include "vec.hpp"

namespace mgl
{

    enum class SupportKind {
        Plane,
        Rod
    };

    struct Scheme1D {
        Vec2 origin{80, 80};  
        double angleDeg = 0;  
        double lengthPx = 700; 
        SupportKind support = SupportKind::Plane;
        int forceDir = 1;                
        const char *angleText = nullptr; 
        int angleAnchor = 0;             
        int angleHDir = 1;               

        Vec2 dir() const;
        /// Схемы 0..9 из методички (последняя цифра варианта).
        static Scheme1D make(int variant, int w, int h);
    };

} // namespace mgl