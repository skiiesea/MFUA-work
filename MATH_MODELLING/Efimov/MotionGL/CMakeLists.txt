cmake_minimum_required(VERSION 3.16)
project(motiongl_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(OpenGL REQUIRED)

option(MOTIONGL_FORCE_FETCH_GLFW "Always build GLFW from sources via FetchContent" OFF)

set(GLFW_LIB "")
if(NOT MOTIONGL_FORCE_FETCH_GLFW)
  find_package(glfw3 CONFIG QUIET)
  if(TARGET glfw3::glfw)
    set(GLFW_LIB glfw3::glfw)   # новые / vcpkg-установки
  elseif(TARGET glfw)
    set(GLFW_LIB glfw)          # старые или ручные установки
  endif()
endif()

if(NOT GLFW_LIB)
  include(FetchContent)
  FetchContent_Declare(glfw URL https://github.com/glfw/glfw/archive/refs/tags/3.4.zip)
  set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(glfw)
  set(GLFW_LIB glfw)
endif()

add_library(motiongl STATIC
  src/gl_loader.cpp
  src/renderer2d.cpp
  src/window.cpp
  src/animator.cpp
  src/scheme1d.cpp
  src/linear_animator.cpp
  src/tube_scheme.cpp
  src/tube_animator.cpp)
target_include_directories(motiongl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(motiongl PUBLIC OpenGL::GL ${GLFW_LIB})

add_executable(demo_projectile demo.cpp)
target_link_libraries(demo_projectile PRIVATE motiongl)
add_executable(demo_linear demo_linear.cpp)
target_link_libraries(demo_linear PRIVATE motiongl)
add_executable(demo_tube demo_tube.cpp)
target_link_libraries(demo_tube PRIVATE motiongl)