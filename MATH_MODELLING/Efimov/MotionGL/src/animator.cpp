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

#include "motiongl/animator.hpp"
#include "motiongl/gl_loader.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <stdexcept>

namespace mgl
{
    namespace
    {

        template <typename... Args>
        std::string fmt(const std::string &f, Args &&...args)         {
            int n = std::snprintf(nullptr, 0, f.c_str(), std::forward<Args>(args)...);
            std::string s(static_cast<size_t>(n), '\0');
            std::snprintf(&s[0], static_cast<size_t>(n) + 1, f.c_str(), std::forward<Args>(args)...);
            return s;
        }
    } // namespace



    Animator AnimatorBuilder::build() const { return Animator(cfg_); }

    Animator::Animator(AnimatorConfig cfg)
        : cfg_(std::move(cfg)), timeScale_(cfg_.timeScale)
    {
        Window::Config wc;
        wc.width = cfg_.width;
        wc.height = cfg_.height;
        wc.title = cfg_.title;
        window_ = std::make_unique<Window>(wc);
        if (!gl::load())
            throw std::runtime_error("failed to load OpenGL functions");
        renderer_ = std::make_unique<Renderer2D>();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);
    }

    Animator::~Animator() = default;

    void Animator::restart() {
        simTime_ = 0.0;
        trail_.clear();
    }

    void Animator::fitScale(const IMotionLaw &law, double duration) {
        if (!cfg_.autoFit) { ppm_ = cfg_.pixelsPerMeter; return; }
        auto [w, h] = window_->framebufferSize();
        double maxX = 1e-9, maxY = 1e-9;
        const int N = 512;
        for (int i = 0; i <= N; ++i) {
            Vec2 p = law.position(duration * i / N);
            maxX = std::max(maxX, p.x);
            maxY = std::max(maxY, p.y);
        }
        double availW = w - cfg_.originPx.x - 70;
        double availH = h - cfg_.originPx.y - 70;
        ppm_ = std::min(availW / maxX, availH / maxY);
        if (!(ppm_ > 0))
            ppm_ = cfg_.pixelsPerMeter;
    }

    Vec2 Animator::project(Vec2 w) const {
        return {cfg_.originPx.x + w.x * ppm_, cfg_.originPx.y + w.y * ppm_};
    }

    void Animator::handleKey(int key, int action) {
        if (action == GLFW_RELEASE)
            return;
        switch (key) {
        case GLFW_KEY_SPACE:
            paused_ = !paused_;
            break;
        case GLFW_KEY_R:
            restart();
            break;
        case GLFW_KEY_EQUAL:
        case GLFW_KEY_KP_ADD:
            timeScale_ = std::min(timeScale_ * 1.25, 16.0);
            break;
        case GLFW_KEY_MINUS:
        case GLFW_KEY_KP_SUBTRACT:
            timeScale_ = std::max(timeScale_ / 1.25, 0.1);
            break;
        case GLFW_KEY_ESCAPE:
            window_->close();
            break;
        default:
            break;
        }
    }

    void Animator::run(const IMotionLaw &law, double duration) {
        window_->onKey([this](int key, int action)
                       { handleKey(key, action); });
        fitScale(law, duration);
        restart();

        double last = glfwGetTime();
        while (!window_->shouldClose()) {
            window_->pollEvents();
            const double now = glfwGetTime();
            const double dt = now - last;
            last = now;

            if (!paused_) {
                simTime_ += dt * timeScale_;
                if (simTime_ >= duration) {
                    if (cfg_.loop)
                        restart();
                    else
                        simTime_ = duration;
                }
            }
            while (static_cast<double>(trail_.size()) * trailDt_ < simTime_)
                trail_.push_back(law.position(static_cast<double>(trail_.size()) * trailDt_));

            render(law, duration);
        }
    }

    void Animator::render(const IMotionLaw &law, double duration) {
        auto [w, h] = window_->framebufferSize();
        if (w <= 0 || h <= 0)
            return;
        glViewport(0, 0, w, h);
        glClearColor(cfg_.background.r, cfg_.background.g, cfg_.background.b, cfg_.background.a);
        glClear(GL_COLOR_BUFFER_BIT);

        Renderer2D &r = *renderer_;
        r.begin(w, h);

        const Vec2 o = cfg_.originPx;

        r.arrow(o, {double(w) - 24, o.y}, cfg_.axisColor, 2.f, 12.f);
        r.arrow(o, {o.x, double(h) - 24}, cfg_.axisColor, 2.f, 12.f);
        r.text("X", {double(w) - 20, o.y + 22}, cfg_.textColor, 14);
        r.text("Y", {o.x + 10, double(h) - 12}, cfg_.textColor, 14);
        r.text("O", {o.x - 22, o.y + 16}, cfg_.textColor, 14);

        char buf[32];
        for (double m = cfg_.tickStepM;; m += cfg_.tickStepM) {
            double px = o.x + m * ppm_;
            if (px > w - 40)
                break;
            r.line({px, o.y - 4}, {px, o.y + 4}, cfg_.axisColor, 1.5f);
            std::snprintf(buf, sizeof(buf), "%.0f", m);
            r.text(buf, {px - 8, o.y - 10}, cfg_.axisColor, 11);
        }
        for (double m = cfg_.tickStepM;; m += cfg_.tickStepM) {
            double py = o.y + m * ppm_;
            if (py > h - 40)
                break;
            r.line({o.x - 4, py}, {o.x + 4, py}, cfg_.axisColor, 1.5f);
            std::snprintf(buf, sizeof(buf), "%.0f", m);
            r.text(buf, {o.x - 36, py + 4}, cfg_.axisColor, 11);
        }

        for (size_t i = 1; i < trail_.size(); ++i)
            r.line(project(trail_[i - 1]), project(trail_[i]), cfg_.trailColor, 2.f);

        Vec2 v0 = law.velocity(0.0);
        r.arrow(o, o + v0 * cfg_.velocityScalePx, cfg_.velocityColor, 2.f, 10.f);

        Vec2 p = law.position(simTime_);
        Vec2 pp = project(p);
        r.filledCircle(pp, 5.f, cfg_.pointColor);
        Vec2 v = law.velocity(simTime_);
        r.arrow(pp, pp + v * cfg_.velocityScalePx, cfg_.velocityColor, 2.f, 10.f);

        double speed = std::hypot(v.x, v.y);
        r.text(fmt("T = %.2f / %.2f S   X = %.1f M   Y = %.1f M   V = %.1f M/S   TIME x%.2f%s",
                   simTime_, duration, p.x, p.y, speed, timeScale_, paused_ ? "   PAUSE" : ""),
               {12, double(h) - 12}, cfg_.textColor, 14);
        r.text("SPACE - PAUSE   R - RESTART   + / - - TIME   ESC - QUIT",
               {12, 22}, cfg_.axisColor, 11);

        r.end();
        window_->swapBuffers();
    }

} // namespace mgl