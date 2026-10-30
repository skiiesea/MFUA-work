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
#include <vector>
#include "motion_law.hpp"
#include "renderer2d.hpp"
#include "window.hpp"

namespace mgl
{

    struct AnimatorConfig
    {
        int width = 1024;
        int height = 640;
        std::string title = "Motion of a material point";

        Vec2 originPx{70.0, 60.0};   
        double pixelsPerMeter = 12.0; 
        bool autoFit = true;          

        double timeScale = 1.0;
        bool loop = true;

        double tickStepM = 5.0;       
        double velocityScalePx = 4.0; 

        Color background = colors::darkBg;
        Color axisColor = colors::gray;
        Color trailColor = colors::cyan;
        Color pointColor = colors::orange;
        Color velocityColor = colors::green;
        Color textColor = colors::white;
    };
    class Animator;
    class AnimatorBuilder {
    public:
        AnimatorBuilder &size(int w, int h) {
            cfg_.width = w;
            cfg_.height = h;
            return *this;
        }
        AnimatorBuilder &title(std::string t) {
            cfg_.title = std::move(t);
            return *this;
        }
        AnimatorBuilder &origin(double x, double y) {
            cfg_.originPx = {x, y};
            return *this;
        }
        AnimatorBuilder &pixelsPerMeter(double v) {
            cfg_.pixelsPerMeter = v;
            cfg_.autoFit = false;
            return *this;
        }
        AnimatorBuilder &autoFit(bool v) {
            cfg_.autoFit = v;
            return *this;
        }
        AnimatorBuilder &timeScale(double v) {
            cfg_.timeScale = v;
            return *this;
        }
        AnimatorBuilder &loop(bool v) {
            cfg_.loop = v;
            return *this;
        }
        AnimatorBuilder &tickStep(double m) {
            cfg_.tickStepM = m;
            return *this;
        }
        Animator build() const;

    private:
        AnimatorConfig cfg_;
    };

    class Animator {
    public:
        explicit Animator(AnimatorConfig cfg);
        Animator(Animator &&) noexcept = default;
        ~Animator();
        Animator(const Animator &) = delete;
        Animator &operator=(const Animator &) = delete;

        void run(const IMotionLaw &law, double durationSec);

        void run(const std::function<double(double)> &fx,
                 const std::function<double(double)> &fy,
                 double durationSec)
        {
            run(FunctionalMotionLaw(fx, fy), durationSec);
        }

    private:
        void handleKey(int key, int action);
        void restart();
        void fitScale(const IMotionLaw &law, double durationSec);
        Vec2 project(Vec2 world) const;
        void render(const IMotionLaw &law, double durationSec);

        AnimatorConfig cfg_;
        std::unique_ptr<Window> window_;
        std::unique_ptr<Renderer2D> renderer_;

        double ppm_ = 10.0;
        double simTime_ = 0.0;
        double timeScale_ = 1.0;
        bool paused_ = false;

        std::vector<Vec2> trail_;
        double trailDt_ = 1.0 / 240.0;
    };

} // namespace mgl