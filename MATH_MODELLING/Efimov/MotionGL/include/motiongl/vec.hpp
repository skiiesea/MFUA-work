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

namespace mgl
{

    struct Vec2 {
        double x = 0.0, y = 0.0;
        constexpr Vec2() = default;
        constexpr Vec2(double x_, double y_) : x(x_), y(y_) {}
        Vec2 operator+(Vec2 o) const { return {x + o.x, y + o.y}; }
        Vec2 operator-(Vec2 o) const { return {x - o.x, y - o.y}; }
        Vec2 operator*(double s) const { return {x * s, y * s}; }
    };

    struct Color {
        float r = 1.f, g = 1.f, b = 1.f, a = 1.f;
    };

    namespace colors
    {
        inline constexpr Color white{1.00f, 1.00f, 1.00f, 1.f};
        inline constexpr Color gray{0.50f, 0.50f, 0.55f, 1.f};
        inline constexpr Color green{0.20f, 0.80f, 0.35f, 1.f};
        inline constexpr Color orange{1.00f, 0.65f, 0.10f, 1.f};
        inline constexpr Color cyan{0.30f, 0.75f, 0.95f, 1.f};
        inline constexpr Color darkBg{0.07f, 0.08f, 0.12f, 1.f};
    } // namespace colors

} // namespace mgl