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
#include <vector>

namespace mgl {

class IScalarLaw1D {
public:
    virtual ~IScalarLaw1D() = default;
    virtual double position(double t) const = 0;
    virtual double velocity(double t, double h = 1e-4) const {
        return (position(t + h) - position(t - h)) / (2.0 * h);
    }
    virtual double durationHint() const { return -1.0; }
};

class AnalyticLaw1D final : public IScalarLaw1D {
public:
    explicit AnalyticLaw1D(std::function<double(double)> f) : f_(std::move(f)) {}
    double position(double t) const override { return f_(t); }
private:
    std::function<double(double)> f_;
};

class SampledLaw1D final : public IScalarLaw1D {
public:
    explicit SampledLaw1D(std::vector<std::pair<double, double>> s) : s_(std::move(s)) {}
    double position(double t) const override {
        if (s_.empty()) return 0.0;
        if (t <= s_.front().first) return s_.front().second;
        if (t >= s_.back().first)  return s_.back().second;
        size_t lo = 0, hi = s_.size() - 1;
        while (hi - lo > 1) {
            size_t mid = (lo + hi) / 2;
            if (s_[mid].first <= t) lo = mid; else hi = mid;
        }
        double dt = s_[hi].first - s_[lo].first;
        double k = dt > 0 ? (t - s_[lo].first) / dt : 0.0;
        return s_[lo].second + k * (s_[hi].second - s_[lo].second);
    }
    double durationHint() const override { return s_.empty() ? 0.0 : s_.back().first; }
private:
    std::vector<std::pair<double, double>> s_;
};

} // namespace mgl