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

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "motiongl/gl_loader.hpp"

namespace mgl::gl
{

    bool load() {
        if (GenVertexArrays)
            return true;
        bool ok = true;
#define MGL_LOAD(fn)                                                   \
    fn = reinterpret_cast<decltype(fn)>(glfwGetProcAddress("gl" #fn)); \
    ok = ok && (fn != nullptr);
        MGL_LOAD(GenVertexArrays)
        MGL_LOAD(BindVertexArray) MGL_LOAD(DeleteVertexArrays)
            MGL_LOAD(GenBuffers) MGL_LOAD(BindBuffer) MGL_LOAD(DeleteBuffers)
                MGL_LOAD(BufferData) MGL_LOAD(EnableVertexAttribArray) MGL_LOAD(VertexAttribPointer)
                    MGL_LOAD(CreateShader) MGL_LOAD(ShaderSource) MGL_LOAD(CompileShader)
                        MGL_LOAD(GetShaderiv) MGL_LOAD(GetShaderInfoLog) MGL_LOAD(DeleteShader)
                            MGL_LOAD(CreateProgram) MGL_LOAD(AttachShader) MGL_LOAD(LinkProgram)
                                MGL_LOAD(GetProgramiv) MGL_LOAD(GetProgramInfoLog) MGL_LOAD(UseProgram)
                                    MGL_LOAD(DeleteProgram) MGL_LOAD(GetUniformLocation) MGL_LOAD(Uniform4f)
#undef MGL_LOAD
                                        return ok;
    }

} // namespace mgl::gl