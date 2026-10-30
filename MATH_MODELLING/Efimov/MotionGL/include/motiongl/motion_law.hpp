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
#include <functional>
#include <utility>
#include "vec.hpp"

namespace mgl
{
    class IMotionLaw {
    public:
        virtual ~IMotionLaw() = default;
        virtual Vec2 position(double t) const = 0;
        virtual Vec2 velocity(double t, double h = 1e-4) const {
            return (position(t + h) - position(t - h)) * (1.0 / (2.0 * h));
        }
    };

    class FunctionalMotionLaw final : public IMotionLaw {
    public:
        using ScalarFn = std::function<double(double)>;
        FunctionalMotionLaw(ScalarFn x, ScalarFn y) : x_(std::move(x)), y_(std::move(y)) {}
        Vec2 position(double t) const override { return {x_(t), y_(t)}; }

    private:
        ScalarFn x_, y_;
    };

} // namespace mgl