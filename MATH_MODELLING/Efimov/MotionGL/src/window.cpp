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

#include "motiongl/window.hpp"
#include <stdexcept>

namespace mgl
{

    Window::Window(const Config &cfg) {
        if (!glfwInit())
            throw std::runtime_error("glfwInit failed");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        handle_ = glfwCreateWindow(cfg.width, cfg.height, cfg.title.c_str(), nullptr, nullptr);
        if (!handle_)
            throw std::runtime_error("glfwCreateWindow failed");
        glfwMakeContextCurrent(handle_);
        glfwSetWindowUserPointer(handle_, this);
        glfwSetKeyCallback(handle_, &Window::keyCallback);
        glfwSwapInterval(1);
    }

    Window::~Window() {
        if (handle_)
            glfwDestroyWindow(handle_);
        glfwTerminate();
    }

    bool Window::shouldClose() const { return glfwWindowShouldClose(handle_) != 0; }
    void Window::pollEvents() { glfwPollEvents(); }
    void Window::swapBuffers() { glfwSwapBuffers(handle_); }
    void Window::close() { glfwSetWindowShouldClose(handle_, 1); }

    std::pair<int, int> Window::framebufferSize() const {
        int w = 0, h = 0;
        glfwGetFramebufferSize(handle_, &w, &h);
        return {w, h};
    }

    void Window::keyCallback(GLFWwindow *w, int key, int /*scancode*/, int action, int /*mods*/) {
        auto *self = static_cast<Window *>(glfwGetWindowUserPointer(w));
        if (self && self->keyFn_)
            self->keyFn_(key, action);
    }

} // namespace mgl