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
#include <utility>
#include <variant>
#include <vector>
#include "law1d.hpp"

namespace mgl
{
    using ScalarInput = std::variant<std::function<double(double)>,
                                     std::vector<std::pair<double, double>>>;

    inline std::unique_ptr<IScalarLaw1D> makeLaw(const ScalarInput &in) {
        if (auto *f = std::get_if<std::function<double(double)>>(&in))
            return std::make_unique<AnalyticLaw1D>(*f);
        return std::make_unique<SampledLaw1D>(
            std::get<std::vector<std::pair<double, double>>>(in));
    }

} // namespace mgl