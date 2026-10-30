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

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#if defined(_WIN32)
#define MGL_CC __stdcall
#else
#define MGL_CC
#endif
typedef char GLchar;
#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif
#ifndef GL_DYNAMIC_DRAW
#define GL_DYNAMIC_DRAW 0x88E8
#endif
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS 0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS 0x8B82
#endif

namespace mgl::gl
{

    inline void(MGL_CC *GenVertexArrays)(GLsizei, GLuint *) = nullptr;
    inline void(MGL_CC *BindVertexArray)(GLuint) = nullptr;
    inline void(MGL_CC *DeleteVertexArrays)(GLsizei, const GLuint *) = nullptr;
    inline void(MGL_CC *GenBuffers)(GLsizei, GLuint *) = nullptr;
    inline void(MGL_CC *BindBuffer)(GLenum, GLuint) = nullptr;
    inline void(MGL_CC *DeleteBuffers)(GLsizei, const GLuint *) = nullptr;
    inline void(MGL_CC *BufferData)(GLenum, long long, const void *, GLenum) = nullptr;
    inline void(MGL_CC *EnableVertexAttribArray)(GLuint) = nullptr;
    inline void(MGL_CC *VertexAttribPointer)(GLuint, GLint, GLenum, GLboolean, GLsizei, const void *) = nullptr;
    inline GLuint(MGL_CC *CreateShader)(GLenum) = nullptr;
    inline void(MGL_CC *ShaderSource)(GLuint, GLsizei, const GLchar *const *, const GLint *) = nullptr;
    inline void(MGL_CC *CompileShader)(GLuint) = nullptr;
    inline void(MGL_CC *GetShaderiv)(GLuint, GLenum, GLint *) = nullptr;
    inline void(MGL_CC *GetShaderInfoLog)(GLuint, GLsizei, GLsizei *, GLchar *) = nullptr;
    inline void(MGL_CC *DeleteShader)(GLuint) = nullptr;
    inline GLuint(MGL_CC *CreateProgram)() = nullptr;
    inline void(MGL_CC *AttachShader)(GLuint, GLuint) = nullptr;
    inline void(MGL_CC *LinkProgram)(GLuint) = nullptr;
    inline void(MGL_CC *GetProgramiv)(GLuint, GLenum, GLint *) = nullptr;
    inline void(MGL_CC *GetProgramInfoLog)(GLuint, GLsizei, GLsizei *, GLchar *) = nullptr;
    inline void(MGL_CC *UseProgram)(GLuint) = nullptr;
    inline void(MGL_CC *DeleteProgram)(GLuint) = nullptr;
    inline GLint(MGL_CC *GetUniformLocation)(GLuint, const GLchar *) = nullptr;
    inline void(MGL_CC *Uniform4f)(GLint, GLfloat, GLfloat, GLfloat, GLfloat) = nullptr;

    bool load();

} // namespace mgl::gl