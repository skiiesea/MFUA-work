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

#include "motiongl/tube_scheme.hpp"

namespace mgl
{

    TubeScheme TubeScheme::make(int v) {
        switch (v) {
            case 0: return {-30, 30}; 
            case 1: return {180, 210}; 
            case 2: return {150, 180}; 
            case 3: return {-30, 0}; 
            case 4: return {0, 30}; 
            case 5: return {30, -30}; 
            case 6: return {0, -30}; 
            case 7: return {180, 150}; 
            case 8: return {210, 180}; 
            case 9: return {30, 0}; 
            default: return {-30, 30};
        }
    }
} // namespace mgl