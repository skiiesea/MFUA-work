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
#include <memory>
#include <string>
#include <utility>
#include "law1d.hpp"
#include "renderer2d.hpp"
#include "scalar_input.hpp"
#include "tube_scheme.hpp"
#include "vec.hpp"
#include "window.hpp"

namespace mgl
{
    struct TubeConfig {
        int width = 1100, height = 640;
        std::string title = "Motion in bent tube ABC";
        int variant = 0;
        double l = 2.0; 
        double timeScale = 1.0;
        bool loop = true;
        double scanT1Max = 30.0; 
        Color background = colors::darkBg;
        Color wallColor = colors::gray;
        Color bodyColor = colors::orange;
        Color axisColor = colors::cyan;
        Color forceColor = colors::green;
        Color textColor = colors::white;
    };

    class TubeAnimator;

    class TubeAnimatorBuilder {
    public:
        TubeAnimatorBuilder &size(int w, int h) {
            cfg_.width = w;
            cfg_.height = h;
            return *this;
        }
        TubeAnimatorBuilder &title(std::string t) {
            cfg_.title = std::move(t);
            return *this;
        }
        TubeAnimatorBuilder &variant(int v) {
            cfg_.variant = v;
            return *this;
        }
        TubeAnimatorBuilder &l(double v) {
            cfg_.l = v;
            return *this;
        }
        TubeAnimatorBuilder &timeScale(double v) {
            cfg_.timeScale = v;
            return *this;
        }
        TubeAnimatorBuilder &loop(bool v) {
            cfg_.loop = v;
            return *this;
        }
        TubeAnimatorBuilder &scanT1Max(double v) {
            cfg_.scanT1Max = v;
            return *this;
        }
        TubeAnimator build() const;

    private:
        TubeConfig cfg_;
    };

    class TubeAnimator {
    public:
        explicit TubeAnimator(TubeConfig cfg);
        TubeAnimator(TubeAnimator &&) noexcept = default;
        ~TubeAnimator();
        TubeAnimator(const TubeAnimator &) = delete;
        TubeAnimator &operator=(const TubeAnimator &) = delete;

        void run(const ScalarInput &seg1, const ScalarInput &seg2, double t2 = -1.0);

    private:
        void handleKey(int key, int action);
        double findTransition(const IScalarLaw1D &s1) const;
        void render(const IScalarLaw1D &s1, const IScalarLaw1D &s2);
        Vec2 project(Vec2 meters) const;

        TubeConfig cfg_;
        std::unique_ptr<Window> window_;
        std::unique_ptr<Renderer2D> renderer_;

        Vec2 e1_{1, 0}, e2_{1, 0};
        double ppm_ = 10.0;
        Vec2 off_{0, 0};
        double tB_ = 0.0, dur2_ = 20.0;
        double simTime_ = 0.0, timeScale_ = 1.0;
        bool paused_ = false;
    };
} // namespace mgl