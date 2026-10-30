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

#include "motiongl/renderer2d.hpp"
#include "motiongl/gl_loader.hpp"
#include <cctype>
#include <cmath>
#include <stdexcept>

namespace mgl
{
    namespace
    {

        constexpr double kPi = 3.14159265358979323846;

        const char *kVertSrc =
            "#version 330 core\n"
            "layout (location = 0) in vec2 aPos;\n"
            "layout (location = 1) in vec4 aColor;\n"
            "uniform vec4 uTransform; // 2/w, 2/h, -1, -1\n"
            "out vec4 vColor;\n"
            "void main() {\n"
            "    vec2 ndc = vec2(aPos.x * uTransform.x + uTransform.z, aPos.y * uTransform.y + uTransform.w);\n"
            "    gl_Position = vec4(ndc, 0.0, 1.0);\n"
            "    vColor = aColor;\n"
            "}\n";

        const char *kFragSrc =
            "#version 330 core\n"
            "in vec4 vColor;\n"
            "out vec4 outColor;\n"
            "void main() { outColor = vColor; }\n";

        GLuint compileShader(GLenum type, const char *src) {
            GLuint sh = gl::CreateShader(type);
            const GLchar *str = src;
            const GLint len = static_cast<GLint>(std::string(src).size());
            gl::ShaderSource(sh, 1, &str, &len);
            gl::CompileShader(sh);
            GLint ok = 0;
            gl::GetShaderiv(sh, GL_COMPILE_STATUS, &ok);
            if (!ok) {
                char log[2048] = {0};
                gl::GetShaderInfoLog(sh, sizeof(log), nullptr, log);
                throw std::runtime_error(std::string("shader compile: ") + log);
            }
            return sh;
        }

        GLuint linkProgram(const char *vs, const char *fs) {
            GLuint v = compileShader(GL_VERTEX_SHADER, vs);
            GLuint f = compileShader(GL_FRAGMENT_SHADER, fs);
            GLuint p = gl::CreateProgram();
            gl::AttachShader(p, v);
            gl::AttachShader(p, f);
            gl::LinkProgram(p);
            GLint ok = 0;
            gl::GetProgramiv(p, GL_LINK_STATUS, &ok);
            if (!ok) {
                char log[2048] = {0};
                gl::GetProgramInfoLog(p, sizeof(log), nullptr, log);
                throw std::runtime_error(std::string("program link: ") + log);
            }
            gl::DeleteShader(v);
            gl::DeleteShader(f);
            return p;
        }

        struct GlyphRec {
            char ch;
            unsigned char rows[7];
        };
        constexpr GlyphRec kGlyphs[] = {
            {'0', {0x0E, 0x11, 0x13, 0x15, 0x19, 0x11, 0x0E}},
            {'1', {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E}},
            {'2', {0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F}},
            {'3', {0x0E, 0x11, 0x01, 0x06, 0x01, 0x11, 0x0E}},
            {'4', {0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02}},
            {'5', {0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E}},
            {'6', {0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E}},
            {'7', {0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08}},
            {'8', {0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E}},
            {'9', {0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C}},
            {'.', {0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C}},
            {'-', {0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00}},
            {'+', {0x00, 0x04, 0x04, 0x1F, 0x04, 0x04, 0x00}},
            {'/', {0x01, 0x02, 0x02, 0x04, 0x08, 0x08, 0x10}},
            {'=', {0x00, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x00}},
            {' ', {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},
            {'A', {0x0E, 0x11, 0x11, 0x1F, 0x11, 0x11, 0x11}},
            {'B', {0x1E, 0x11, 0x11, 0x1E, 0x11, 0x11, 0x1E}},
            {'C', {0x0E, 0x11, 0x10, 0x10, 0x10, 0x11, 0x0E}},
            {'E', {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x1F}},
            {'F', {0x1F, 0x10, 0x10, 0x1E, 0x10, 0x10, 0x10}},
            {'I', {0x0E, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0E}},
            {'M', {0x11, 0x1B, 0x15, 0x15, 0x11, 0x11, 0x11}},
            {'O', {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E}},
            {'P', {0x1E, 0x11, 0x11, 0x1E, 0x10, 0x10, 0x10}},
            {'Q', {0x0E, 0x11, 0x11, 0x11, 0x15, 0x12, 0x0D}},
            {'R', {0x1E, 0x11, 0x11, 0x1E, 0x14, 0x12, 0x11}},
            {'S', {0x0F, 0x10, 0x10, 0x0E, 0x01, 0x01, 0x1E}},
            {'T', {0x1F, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04}},
            {'U', {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E}},
            {'V', {0x11, 0x11, 0x11, 0x11, 0x0A, 0x0A, 0x04}},
            {'X', {0x11, 0x11, 0x0A, 0x04, 0x0A, 0x11, 0x11}},
            {'Y', {0x11, 0x11, 0x0A, 0x04, 0x04, 0x04, 0x04}},
        };
        const unsigned char *glyphRows(char ch) {
            ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
            for (const auto &g : kGlyphs)
                if (g.ch == ch)
                    return g.rows;
            return nullptr;
        }

    } // namespace

    Renderer2D::Renderer2D() {
        program_ = linkProgram(kVertSrc, kFragSrc);
        uTransform_ = gl::GetUniformLocation(program_, "uTransform");
        gl::GenVertexArrays(1, &vao_);
        gl::GenBuffers(1, &vbo_);
        gl::BindVertexArray(vao_);
        gl::BindBuffer(GL_ARRAY_BUFFER, vbo_);
        gl::EnableVertexAttribArray(0);
        gl::VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * static_cast<GLsizei>(sizeof(float)), nullptr);
        gl::EnableVertexAttribArray(1);
        gl::VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * static_cast<GLsizei>(sizeof(float)),
                                reinterpret_cast<const void *>(2 * sizeof(float)));
    }

    Renderer2D::~Renderer2D() {
        if (vbo_) gl::DeleteBuffers(1, &vbo_);
        if (vao_) gl::DeleteVertexArrays(1, &vao_);
        if (program_) gl::DeleteProgram(program_);
    }

    void Renderer2D::begin(int w, int h) {
        width_ = w;
        height_ = h;
        vertices_.clear();
    }

    void Renderer2D::pushVertex(Vec2 p, const Color &c) {
        vertices_.push_back(static_cast<float>(p.x));
        vertices_.push_back(static_cast<float>(p.y));
        vertices_.push_back(c.r);
        vertices_.push_back(c.g);
        vertices_.push_back(c.b);
        vertices_.push_back(c.a);
    }

    void Renderer2D::quad(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, const Color &c) {
        pushVertex(p0, c);
        pushVertex(p1, c);
        pushVertex(p2, c);
        pushVertex(p0, c);
        pushVertex(p2, c);
        pushVertex(p3, c);
    }

    void Renderer2D::line(Vec2 a, Vec2 b, const Color &c, float thickness) {
        Vec2 d = b - a;
        double len = std::hypot(d.x, d.y);
        if (len < 1e-9) return;
        Vec2 n{-d.y / len * thickness * 0.5, d.x / len * thickness * 0.5};
        quad(a + n, b + n, b - n, a - n, c);
    }

    void Renderer2D::arrow(Vec2 a, Vec2 b, const Color &c, float thick, float headLen) {
        Vec2 d = b - a;
        double len = std::hypot(d.x, d.y);
        if (len < 1e-6) return;
        Vec2 dir = d * (1.0 / len);
        Vec2 n{-dir.y, dir.x};
        float hw = headLen * 0.45f;
        Vec2 base = b - dir * headLen;
        line(a, base, c, thick);
        pushVertex(b, c);
        pushVertex(base + n * hw, c);
        pushVertex(base - n * hw, c);
    }

    void Renderer2D::filledCircle(Vec2 c0, float radius, const Color &c, int seg) {
        for (int i = 0; i < seg; ++i) {
            double a0 = 2 * kPi * i / seg, a1 = 2 * kPi * (i + 1) / seg;
            pushVertex(c0, c);
            pushVertex({c0.x + radius * std::cos(a0), c0.y + radius * std::sin(a0)}, c);
            pushVertex({c0.x + radius * std::cos(a1), c0.y + radius * std::sin(a1)}, c);
        }
    }

    void Renderer2D::filledQuad(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, const Color &c) { quad(p0, p1, p2, p3, c); }

    void Renderer2D::text(const std::string &s, Vec2 top, const Color &c, float charHeight) {
        const float u = charHeight / 7.f;
        double x = top.x;
        for (char ch : s) {
            const unsigned char *rows = glyphRows(ch);
            if (rows) {
                for (int r = 0; r < 7; ++r)
                    for (int col = 0; col < 5; ++col)
                        if (rows[r] & (1 << (4 - col))) {
                            double x0 = x + col * u, y0 = top.y - r * u;
                            quad({x0, y0}, {x0 + u, y0}, {x0 + u, y0 - u}, {x0, y0 - u}, c);
                        }
            }
            x += 6.0 * u;
        }
    }

    void Renderer2D::end() {
        if (vertices_.empty()) return;
        gl::UseProgram(program_);
        gl::Uniform4f(uTransform_, 2.f / float(width_), 2.f / float(height_), -1.f, -1.f);
        gl::BindVertexArray(vao_);
        gl::BindBuffer(GL_ARRAY_BUFFER, vbo_);
        gl::BufferData(GL_ARRAY_BUFFER,
                       static_cast<long long>(vertices_.size() * sizeof(float)),
                       vertices_.data(), GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_.size() / 6));
    }

} // namespace mgl