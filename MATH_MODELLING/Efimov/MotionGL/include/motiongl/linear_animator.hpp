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
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "law1d.hpp"
#include "renderer2d.hpp"
#include "scheme1d.hpp"
#include "window.hpp"

namespace mgl
{

    struct LinearConfig
    {
        int width = 1024, height = 640;
        std::string title = "Motion along Ox";
        int variant = 1;
        double timeScale = 1.0;
        bool loop = true;
        Color background = colors::darkBg;
        Color supportColor = colors::gray;
        Color bodyColor = colors::orange;
        Color forceColor = colors::green;
        Color textColor = colors::white;
    };

    class LinearAnimator; 

    class LinearAnimatorBuilder {
    public:
        LinearAnimatorBuilder &size(int w, int h) {
            cfg_.width = w;
            cfg_.height = h;
            return *this;
        }
        LinearAnimatorBuilder &title(std::string t) {
            cfg_.title = std::move(t);
            return *this;
        }
        LinearAnimatorBuilder &variant(int v) {
            cfg_.variant = v;
            return *this;
        }
        LinearAnimatorBuilder &timeScale(double v) {
            cfg_.timeScale = v;
            return *this;
        }
        LinearAnimatorBuilder &loop(bool v) {
            cfg_.loop = v;
            return *this;
        }
        LinearAnimator build() const;

    private:
        LinearConfig cfg_;
    };

    class LinearAnimator {
    public:
        explicit LinearAnimator(LinearConfig cfg);
        LinearAnimator(LinearAnimator &&) noexcept = default;
        ~LinearAnimator();
        LinearAnimator(const LinearAnimator &) = delete;
        LinearAnimator &operator=(const LinearAnimator &) = delete;

        void run(const IScalarLaw1D &law, double duration);
        void run(std::function<double(double)> xOfT, double duration);
        void run(std::vector<std::pair<double, double>> samples);

    private:
        void handleKey(int key, int action);
        void fitScale(const IScalarLaw1D &law, double duration);
        void render(const IScalarLaw1D &law, double duration);

        LinearConfig cfg_;
        std::unique_ptr<Window> window_;
        std::unique_ptr<Renderer2D> renderer_;
        Scheme1D scheme_;
        double ppm_ = 10.0; 
        double tail_ = 0.0; 
        double simTime_ = 0.0;
        double timeScale_ = 1.0;
        bool paused_ = false;
    };

} // namespace mgl