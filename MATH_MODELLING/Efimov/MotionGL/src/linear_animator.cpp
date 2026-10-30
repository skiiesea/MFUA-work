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

#include "motiongl/linear_animator.hpp"
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
        std::string fmt(const std::string &f, Args &&...args) {
            int n = std::snprintf(nullptr, 0, f.c_str(), std::forward<Args>(args)...);
            std::string s(static_cast<size_t>(n), '\0');
            std::snprintf(&s[0], static_cast<size_t>(n) + 1, f.c_str(), std::forward<Args>(args)...);
            return s;
        }
    } // namespace

    LinearAnimator LinearAnimatorBuilder::build() const { return LinearAnimator(cfg_); }

    LinearAnimator::LinearAnimator(LinearConfig cfg)
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

    LinearAnimator::~LinearAnimator() = default;

    void LinearAnimator::run(std::function<double(double)> xOfT, double duration) {
        run(AnalyticLaw1D(std::move(xOfT)), duration);
    }

    void LinearAnimator::run(std::vector<std::pair<double, double>> samples) {
        SampledLaw1D law(std::move(samples));
        run(law, law.durationHint());
    }

    void LinearAnimator::handleKey(int key, int action) {
        if (action == GLFW_RELEASE)
            return;
        switch (key) {
        case GLFW_KEY_SPACE:
            paused_ = !paused_;
            break;
        case GLFW_KEY_R:
            simTime_ = 0.0;
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

    void LinearAnimator::fitScale(const IScalarLaw1D &law, double duration) {
        double lo = 0.0, hi = 0.5;
        const int N = 1024;
        for (int i = 0; i <= N; ++i) {
            double x = law.position(duration * i / N);
            lo = std::min(lo, x);
            hi = std::max(hi, x);
        }
        ppm_ = (scheme_.lengthPx - 90.0) / hi;
        tail_ = lo < 0 ? std::min(60.0, -lo * ppm_) : 0.0;
    }

    void LinearAnimator::run(const IScalarLaw1D &law, double duration) {
        window_->onKey([this](int key, int action)
                       { handleKey(key, action); });
        auto [w, h] = window_->framebufferSize();
        scheme_ = Scheme1D::make(cfg_.variant, w, h);
        fitScale(law, duration);
        simTime_ = 0.0;

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
                        simTime_ = 0.0;
                    else
                        simTime_ = duration;
                }
            }
            render(law, duration);
        }
    }

    void LinearAnimator::render(const IScalarLaw1D &law, double duration) {
        auto [w, h] = window_->framebufferSize();
        if (w <= 0 || h <= 0)
            return;
        glViewport(0, 0, w, h);
        glClearColor(cfg_.background.r, cfg_.background.g, cfg_.background.b, cfg_.background.a);
        glClear(GL_COLOR_BUFFER_BIT);

        Renderer2D &r = *renderer_;
        r.begin(w, h);

        const Vec2 e = scheme_.dir();
        const Vec2 perp{-e.y, e.x};
        const Vec2 down = perp.y < 0 ? perp : Vec2{-perp.x, -perp.y};
        const Vec2 up{-down.x, -down.y};
        const Vec2 O = scheme_.origin;
        const Vec2 tip = O + e * scheme_.lengthPx;
        const Color &sc = cfg_.supportColor;

        if (scheme_.support == SupportKind::Plane)
        {
            Vec2 start = O - e * tail_;
            r.line(start, tip, sc, 2.f);
            for (double s = 8; s < scheme_.lengthPx + tail_; s += 14) {
                Vec2 p = start + e * s;
                r.line(p, p + down * 10 - e * 7, sc, 1.5f); 
            }
        }
        else { 
            r.line(O + perp * 3, tip + perp * 3, sc, 1.5f);
            r.line(O - perp * 3, tip - perp * 3, sc, 1.5f);
            Vec2 q = O - e * 6;
            r.line(q - perp * 26, q + perp * 26, sc, 2.5f);
            for (double s = -24; s <= 24; s += 8)
                r.line(q + perp * s, q + perp * s - e * 9, sc, 1.5f);
        }

        r.arrow(O, tip, sc, 2.f, 12.f);
        r.text("X", tip + e * 14 + up * 16, cfg_.textColor, 14);
        r.text("O", O - e * 10 - up * 4 + Vec2{-14, -18}, cfg_.textColor, 14);

        if (scheme_.angleText) {
            Vec2 A = scheme_.angleAnchor == 0 ? O : tip;
            double hd = double(scheme_.angleHDir);
            for (double x = 0; x < 90; x += 12)
                r.line(A + Vec2{hd * x, 0}, A + Vec2{hd * std::min(x + 7, 90.0), 0}, sc, 1.5f);
            r.text(scheme_.angleText, A + Vec2{hd * 52 - 6, 16}, cfg_.textColor, 12);
        }

        double x = law.position(simTime_);
        Vec2 p = O + e * (x * ppm_);
        Vec2 c = scheme_.support == SupportKind::Plane ? p + up * 13 : p;
        const double a = 23, b = 13; 
        r.filledQuad(c + e * a + perp * b, c + e * a - perp * b,
                     c - e * a - perp * b, c - e * a + perp * b, cfg_.bodyColor);
        r.line(c + e * a + perp * b, c + e * a - perp * b, cfg_.textColor, 1.5f);
        r.line(c + e * a - perp * b, c - e * a - perp * b, cfg_.textColor, 1.5f);
        r.line(c - e * a - perp * b, c - e * a + perp * b, cfg_.textColor, 1.5f);
        r.line(c - e * a + perp * b, c + e * a + perp * b, cfg_.textColor, 1.5f);
        r.text("M", c + up * 40, cfg_.textColor, 13);

        double fd = double(scheme_.forceDir);
        Vec2 fs = c + e * fd * (a + 3);
        r.arrow(fs, fs + e * fd * 36, cfg_.forceColor, 2.5f, 10.f);
        r.text("F", fs + e * fd * 46 + up * 12, cfg_.forceColor, 13);

        r.text(fmt("T = %.2f / %.1f S   X = %.3f M   V = %.3f M/S   TIME x%.2f%s",
                   simTime_, duration, x, law.velocity(simTime_), timeScale_,
                   paused_ ? "   PAUSE" : ""),
               {12, double(h) - 12}, cfg_.textColor, 14);
        r.text("SPACE - PAUSE   R - RESTART   + / - - TIME   ESC - QUIT",
               {12, 22}, cfg_.supportColor, 11);

        r.end();
        window_->swapBuffers();
    }

} // namespace mgl