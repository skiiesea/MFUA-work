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

#include "motiongl/tube_animator.hpp"
#include "motiongl/gl_loader.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <stdexcept>

namespace mgl
{
    namespace
    {
        constexpr double kPi = 3.14159265358979323846;
        Vec2 dirFromDeg(double a) { return {std::cos(a * kPi / 180), std::sin(a * kPi / 180)}; }

        template <typename... Args>
        std::string fmt(const std::string &f, Args &&...args) {
            int n = std::snprintf(nullptr, 0, f.c_str(), std::forward<Args>(args)...);
            std::string s(static_cast<size_t>(n), '\0');
            std::snprintf(&s[0], static_cast<size_t>(n) + 1, f.c_str(), std::forward<Args>(args)...);
            return s;
        }
    } // namespace

    TubeAnimator TubeAnimatorBuilder::build() const { return TubeAnimator(cfg_); }

    TubeAnimator::TubeAnimator(TubeConfig cfg)
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

    TubeAnimator::~TubeAnimator() = default;

    Vec2 TubeAnimator::project(Vec2 m) const { return {off_.x + m.x * ppm_, off_.y + m.y * ppm_}; }

    double TubeAnimator::findTransition(const IScalarLaw1D &s1) const {
        const double l = cfg_.l;
        const double hint = s1.durationHint();
        const double T = hint > 0 ? hint : cfg_.scanT1Max;
        const int N = 2000;
        for (int i = 1; i <= N; ++i) {
            double t = T * i / N;
            if (s1.position(t) >= l) {
                double lo = t - T / N, hi = t;
                for (int k = 0; k < 60; ++k) {
                    double mid = 0.5 * (lo + hi);
                    if (s1.position(mid) >= l)
                        hi = mid;
                    else
                        lo = mid;
                }
                return hi;
            }
        }
        return T; 
    }

    void TubeAnimator::handleKey(int key, int action) {
        if (action == GLFW_RELEASE) return;
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

    void TubeAnimator::run(const ScalarInput &in1, const ScalarInput &in2, double t2) {
        auto s1 = makeLaw(in1);
        auto s2 = makeLaw(in2);

        window_->onKey([this](int k, int a) { handleKey(k, a); });

        const TubeScheme sch = TubeScheme::make(cfg_.variant);
        e1_ = dirFromDeg(sch.abDeg);
        e2_ = dirFromDeg(sch.bcDeg);

        tB_ = findTransition(*s1);
        const double hint2 = s2->durationHint();
        dur2_ = t2 > 0 ? t2 : (hint2 > 0 ? hint2 : 20.0);

        auto [w, h] = window_->framebufferSize();
        double hi2 = 0.5, lo2 = 0.0;
        for (int i = 0; i <= 1024; ++i) {
            double x = s2->position(dur2_ * i / 1024);
            hi2 = std::max(hi2, x);
            lo2 = std::min(lo2, x);
        }
        Vec2 A{-e1_.x * cfg_.l, -e1_.y * cfg_.l};
        Vec2 C{e2_.x * hi2, e2_.y * hi2};
        Vec2 D{e2_.x * lo2, e2_.y * lo2};
        double minX = std::min({A.x, 0.0, C.x, D.x}), maxX = std::max({A.x, 0.0, C.x, D.x});
        double minY = std::min({A.y, 0.0, C.y, D.y}), maxY = std::max({A.y, 0.0, C.y, D.y});
        const double M = 80.0;
        ppm_ = std::min((w - 2 * M) / std::max(maxX - minX, 1e-6),
                        (h - 2 * M) / std::max(maxY - minY, 1e-6));
        off_ = {w / 2.0 - ppm_ * 0.5 * (minX + maxX), h / 2.0 - ppm_ * 0.5 * (minY + maxY)};

        simTime_ = 0.0;
        double last = glfwGetTime();
        const double total = tB_ + dur2_;
        while (!window_->shouldClose()) {
            window_->pollEvents();
            const double now = glfwGetTime();
            const double dt = now - last;
            last = now;
            if (!paused_) {
                simTime_ += dt * timeScale_;
                if (simTime_ >= total)
                    simTime_ = cfg_.loop ? 0.0 : total;
            }
            render(*s1, *s2);
        }
    }

    void TubeAnimator::render(const IScalarLaw1D &s1, const IScalarLaw1D &s2) {
        auto [w, h] = window_->framebufferSize();
        if (w <= 0 || h <= 0) return;
        glViewport(0, 0, w, h);
        glClearColor(cfg_.background.r, cfg_.background.g, cfg_.background.b, cfg_.background.a);
        glClear(GL_COLOR_BUFFER_BIT);

        Renderer2D &r = *renderer_;
        r.begin(w, h);
        const Color &wc = cfg_.wallColor;

        auto drawTube = [&](Vec2 S, Vec2 E) { 
            Vec2 d = E - S;
            double len = std::hypot(d.x, d.y);
            if (len < 1e-6) return;
            Vec2 e{d.x / len, d.y / len};
            Vec2 perp{-e.y, e.x};
            Vec2 down = perp.y < 0 ? perp : Vec2{-perp.x, -perp.y};
            r.line(S + perp * 9, E + perp * 9, wc, 2.f);
            r.line(S - perp * 9, E - perp * 9, wc, 2.f);
            r.line(S + perp * 9, S - perp * 9, wc, 2.f); 
            for (double s = 6; s < len; s += 14) {
                Vec2 p = S + e * s - down * 9;
                r.line(p, p + down * 10 - e * 7, wc, 1.5f);
            }
        };
        auto drawAngle = [&](Vec2 P, double hdir) { 
            for (double x = 0; x < 80; x += 12)
                r.line(P + Vec2{hdir * x, 0}, P + Vec2{hdir * std::min(x + 7, 80.0), 0}, wc, 1.5f);
            r.text("30", P + Vec2{hdir * 36 - 7, 15}, cfg_.textColor, 12);
        };

        const Vec2 Bpx = project({0, 0});
        const Vec2 Apx = project({-e1_.x * cfg_.l, -e1_.y * cfg_.l});

        drawTube(Apx, Bpx);
        Vec2 Cpx = Bpx + e2_ * (ppm_ * 1.0); 
        (void)Cpx;

        double axisLen = ppm_ * std::max(1.0, cfg_.l); 
        Vec2 axisTip = Bpx + e2_ * axisLen;
        axisTip = Bpx + e2_ * (ppm_ * 4.0);
        r.arrow(Bpx, axisTip, cfg_.axisColor, 1.5f, 10.f);
        r.text("X", axisTip + e2_ * 12 + Vec2{0, 14}, cfg_.textColor, 14);

        drawTube(Bpx, Bpx + e2_ * (ppm_ * 3.5));

        r.text("A", Apx + Vec2{-6, 16}, cfg_.textColor, 14);
        r.text("B", Bpx + Vec2{-6, -26}, cfg_.textColor, 14);
        r.text("C", Bpx + e2_ * (ppm_ * 3.5) + Vec2{-6, 16}, cfg_.textColor, 14);

        if (std::fabs(e1_.y) > 0.01) {
            bool aLower = Apx.y < Bpx.y;
            Vec2 P = aLower ? Apx : Bpx;
            double hdir = aLower ? (Bpx.x > Apx.x ? 1 : -1) : (Apx.x > Bpx.x ? 1 : -1);
            drawAngle(P, hdir);
        }
        if (std::fabs(e2_.y) > 0.01) {
            Vec2 Cend = Bpx + e2_ * (ppm_ * 3.5);
            bool cLower = Cend.y < Bpx.y;
            Vec2 P = cLower ? Cend : Bpx;
            double hdir = cLower ? (Bpx.x > Cend.x ? 1 : -1) : (Cend.x > Bpx.x ? 1 : -1);
            drawAngle(P, hdir);
        }

        double t = simTime_;
        Vec2 e = e1_;
        double coord = 0.0, vel = 0.0;
        Vec2 posM;
        bool onFirst = t < tB_;
        if (onFirst) {
            coord = std::min(std::max(s1.position(t), 0.0), cfg_.l);
            vel = s1.velocity(t);
            posM = {-e1_.x * (cfg_.l - coord), -e1_.y * (cfg_.l - coord)};
        }
        else {
            coord = s2.position(t - tB_);
            vel = s2.velocity(t - tB_);
            posM = {e2_.x * coord, e2_.y * coord};
            e = e2_;
        }
        Vec2 p = project(posM);

        Vec2 perp{-e.y, e.x};
        const double a = 14, b = 6;
        r.filledQuad(p + e * a + perp * b, p + e * a - perp * b,
                     p - e * a - perp * b, p - e * a + perp * b, cfg_.bodyColor);
        r.line(p + e * a + perp * b, p + e * a - perp * b, cfg_.textColor, 1.5f);
        r.line(p + e * a - perp * b, p - e * a - perp * b, cfg_.textColor, 1.5f);
        r.line(p - e * a - perp * b, p - e * a + perp * b, cfg_.textColor, 1.5f);
        r.line(p - e * a + perp * b, p + e * a + perp * b, cfg_.textColor, 1.5f);

        if (std::fabs(vel) > 1e-3) {
            Vec2 vd = e * (vel > 0 ? 1 : -1);
            double L = std::min(46.0, 8.0 + 10.0 * std::fabs(vel));
            r.arrow(p + vd * (a + 2), p + vd * (a + 2 + L), cfg_.forceColor, 2.f, 9.f);
        }

        r.text(fmt("T = %.2f S (T_B = %.2f)   SEG = %s   %s = %.3f M   V = %.3f M/S   TIME x%.2f%s",
                   t, tB_, onFirst ? "AB" : "BC", onFirst ? "S" : "X", coord, vel,
                   timeScale_, paused_ ? "   PAUSE" : ""),
               {12, double(h) - 12}, cfg_.textColor, 14);
        r.text("SPACE - PAUSE   R - RESTART   + / - - TIME   ESC - QUIT",
               {12, 22}, cfg_.wallColor, 11);

        r.end();
        window_->swapBuffers();
    }

} // namespace mgl