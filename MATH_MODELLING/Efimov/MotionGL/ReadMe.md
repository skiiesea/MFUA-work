# MotionGL - OpenGL Animation Library for Classical Mechanics Coursework

> 🇷🇺 Русская версия документации: [ReadMe_RU.md](ReadMe_RU.md)

**MotionGL** is a small C++17 / OpenGL 3.3 library that turns the result of your mechanics homework into an interactive animation. You solve the differential equation of motion (analytically or numerically - Euler, Runge–Kutta, …), pass the resulting law of motion to the library **as a function `x(t)`/`y(t)` or as a quantized set of samples `{t, x}`**, and the library draws the moving body, axes, supports, force/velocity arrows and a HUD, so you can visually verify whether your mathematical model is adequate.

The library was written for students: the API is intentionally minimal, and a complete animation typically takes **10–20 lines of your code**.

---

## Table of contents

- [MotionGL - OpenGL Animation Library for Classical Mechanics Coursework](#motiongl---opengl-animation-library-for-classical-mechanics-coursework)
  - [Table of contents](#table-of-contents)
  - [Features](#features)
  - [Requirements](#requirements)
  - [Project structure](#project-structure)
  - [Building](#building)
    - [Windows / Visual Studio](#windows--visual-studio)
    - [Windows / MinGW](#windows--mingw)
    - [Linux](#linux)
    - [macOS](#macos)
    - [CMake options](#cmake-options)
  - [Running the demos](#running-the-demos)
  - [Keyboard controls](#keyboard-controls)
  - [Library usage (student guide)](#library-usage-student-guide)
    - [1) 2D motion - `Animator`](#1-2d-motion---animator)
    - [2) 1D motion along `Ox` - `LinearAnimator`](#2-1d-motion-along-ox---linearanimator)
    - [3) Bent tube `ABC` - `TubeAnimator`](#3-bent-tube-abc---tubeanimator)
  - [Schemes reference](#schemes-reference)
    - [`LinearAnimator` - variants 0…9](#linearanimator---variants-09)
    - [`TubeAnimator` - variants 0…9 (directions of `A→B` and `B→C`)](#tubeanimator---variants-09-directions-of-ab-and-bc)
  - [Architecture \& design patterns](#architecture--design-patterns)
  - [API cheat-sheet](#api-cheat-sheet)
  - [Troubleshooting](#troubleshooting)
  - [License](#license)

---

## Features

- **Three independent animators** covering typical coursework assignments:
  - **`Animator`** - planar motion `x(t), y(t)` (e.g. a projectile thrown at an angle with linear air drag). Draws the `Ox`/`Oy` axes with the origin near the bottom‑left corner, tick marks with values, the trajectory trail, the
    moving point, the initial and current velocity vectors and a HUD.
  - **`LinearAnimator`** - rectilinear motion of a body (drawn as a rectangle) along an axis `Ox` for **10 scheme variants** (horizontal surface, inclined plane, vertical rod, horizontal rod with a wall mount; the axis direction and the origin position depend on the variant).
  - **`TubeAnimator`** - motion of a load in a **bent tube `ABC`** lying in a vertical plane (two segments, each inclined at 30° or horizontal, 10 scheme variants). The library **automatically determines the transition time `t_B`** from the condition `s₁(t_B) = |AB|`.
- **Flexible input**: every law of motion can be given either as an analytic function (`std::function<double(double)>`) or as a vector of pairs `{t, value}` produced by a numerical method (linear interpolation is used between samples). The two forms can be mixed per segment.
- **Automatic scale fitting** - the trajectory always fits the window.
- **Interactive playback**: pause, restart, time acceleration/slow‑down, loop.
- **Minimal dependencies**: only [GLFW](https://www.glfw.org/). All required OpenGL 3.3 core functions are loaded through a built‑in loader (`gl_loader`) - no GLEW/GLAD needed.
- **Modern C++17** and classic design patterns (Strategy, Adapter, Builder, Facade, RAII, Observer) - the code base itself can be used as a reference of clean OpenGL project organization.

## Requirements

| Component            | Version / notes                                             |
|----------------------|-------------------------------------------------------------|
| CMake                | ≥ 3.16                                                      |
| C++ compiler         | MSVC 2019+, MinGW‑w64, GCC 9+, Clang 10+ (C++17 required)   |
| GLFW                 | any 3.3+; if not found, CMake downloads it automatically    |
| OpenGL               | 3.3 core profile (any GPU/driver from the last ~15 years)   |
| Linux only (if GLFW is built from source) | `libgl1-mesa-dev`, X11 dev packages (see below) |

## Project structure

```
MotionGL/
├── CMakeLists.txt
├── ReadMe.md                  # this file (English)
├── ReadMe_RU.md               # Russian version
├── img/                       # figures used in the documentation
├── include/motiongl/
│   ├── vec.hpp                # Vec2, Color
│   ├── motion_law.hpp         # Strategy: IMotionLaw + FunctionalMotionLaw (2D)
│   ├── law1d.hpp              # Strategy: IScalarLaw1D, AnalyticLaw1D, SampledLaw1D
│   ├── scalar_input.hpp       # ScalarInput = function | vector<{t,x}>
│   ├── gl_loader.hpp          # minimal OpenGL 3.3 loader
│   ├── renderer2d.hpp         # batched 2D screen-space renderer + bitmap font
│   ├── window.hpp             # RAII GLFW window + Observer (keyboard)
│   ├── animator.hpp           # 2D motion facade + builder
│   ├── scheme1d.hpp           # geometry of the 10 "straight axis" schemes
│   ├── linear_animator.hpp    # 1D motion facade + builder
│   ├── tube_scheme.hpp        # geometry of the 10 bent-tube schemes
│   └── tube_animator.hpp      # bent-tube facade + builder
├── src/                       # implementations of the above
└── examples/
    ├── demo_projectile.cpp    # 2D demo (lecture formulas)
    ├── demo_variant.cpp       # 1D demo, RK4 + sampled input
    └── demo_tube.cpp          # bent-tube demo, mixed input
```

## Building

The build is identical on all platforms; only the toolchain differs.

### Windows / Visual Studio

```bat
cmake -S . -B build
cmake --build build --config Release --parallel
build\Release\demo_projectile.exe
```

### Windows / MinGW

```bat
cmake -S . -B build
cmake --build build --parallel
build\demo_projectile.exe
```

### Linux

```bash
sudo apt install libgl1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/demo_projectile
```

### macOS

```bash
brew install glfw
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
./build/demo_projectile
```

### CMake options

| Option                       | Default | Description                                   |
|------------------------------|---------|-----------------------------------------------|
| `MOTIONGL_FORCE_FETCH_GLFW`  | `OFF`   | ignore any system GLFW and build it from source (useful when the system installation is broken) |

Example: `cmake -S . -B build -DMOTIONGL_FORCE_FETCH_GLFW=ON`.

## Running the demos

| Executable        | What it shows                                                              | Arguments            |
|-------------------|----------------------------------------------------------------------------|----------------------|
| `demo_projectile` | 2D motion: projectile with linear drag (formulas from the lecture)         | -                    |
| `demo_variant`    | 1D motion along `Ox`, body = rectangle; ODE solved inside the demo by RK4  | scheme number `0…9`  |
| `demo_tube`       | motion in the bent tube `ABC`; segment `AB` = function, `BC` = samples     | scheme number `0…9`  |

Examples:

```bash
demo_variant 3      # inclined plane 60°, axis up-right
demo_tube 5         # "roof"-shaped tube
```

## Keyboard controls

| Key        | Action                          |
|------------|---------------------------------|
| `Space`    | pause / resume                  |
| `R`        | restart the animation           |
| `+` / `-`  | speed up / slow down the time   |
| `Esc`      | close the window                |

---

## Library usage (student guide)

The typical workflow is:

1. Derive and solve your equation of motion (analytically, or numerically in your own program).
2. Export the result either as a **formula** or as a **table of samples** `{t, x}` (e.g. from Euler / RK4).
3. Pass it to one of the animators and watch. If the animation contradicts your `x(t)` plot or physical common sense (the body moves "backwards", accelerates forever, never stops…) - your model contains an error (wrong sign of friction, wrong axis direction, …). That is exactly the point of the animation.

### 1) 2D motion - `Animator`

```cpp
#include <motiongl/animator.hpp>
using namespace mgl;

// x(t) and y(t) - your solution:
auto fx = [](double t) { return 125.0 * (1.0 - std::exp(-0.1 * t)); };
auto fy = [](double t) { return 1197.5 * (1.0 - std::exp(-0.1 * t)) - 98.1 * t; };

Animator anim = AnimatorBuilder()
    .size(1100, 640).title("Projectile").tickStep(5).autoFit(true)
    .build();

anim.run(fx, fy, 4.2);          // two functions + duration [s]
```

You may instead implement the `IMotionLaw` interface and pass your own class, e.g. one that also returns the velocity analytically.

### 2) 1D motion along `Ox` - `LinearAnimator`

The body is drawn as a rectangle sliding along the axis; the scheme number (0–9) selects the axis direction, the origin position and the support type (see [Schemes reference](#schemes-reference)).

```cpp
#include <motiongl/linear_animator.hpp>
using namespace mgl;

// variant A: analytic law
anim.run([](double t) { return 0.2 + 0.1*t + 4.0*t*t; }, 20.0);

// variant B: samples from a numerical solver (linear interpolation inside)
std::vector<std::pair<double,double>> samples = {{0,0.2},{0.01,0.201},...};
anim.run(samples);
```

A complete working example with an RK4 solver is in [`examples/demo.cpp`](examples/demo.cpp).

![1D schemes](img/linear_schemes.png)

### 3) Bent tube `ABC` - `TubeAnimator`

The motion has one coordinate axis, but it **changes direction at `B`**, because the two segments are solved separately. Therefore the animator accepts **two inputs** - the law on `AB` (coordinate `s` from `A`) and the law on `BC` (coordinate `x = BD` from `B`). Each input may again be a function or a set of pairs. The transition time `t_B` is **found automatically** (scan + bisection of `s₁(t) = l`), and the second law is then interpreted in the shifted time `τ = t − t_B`.

```cpp
#include <motiongl/tube_animator.hpp>
using namespace mgl;

TubeAnimator anim = TubeAnimatorBuilder()
    .size(1100, 640).variant(4).l(2.0)   // l = |AB|, m
    .build();

auto   seg1 = [](double t) { return t + t*t; };      // s(t) on AB, from A
std::vector<std::pair<double,double>> seg2 = ...;    // x(τ) on BC, from B

anim.run(seg1, seg2);               // function + samples (any combination)
```

The HUD shows `t`, the detected `t_B`, the current segment (`AB`/`BC`), the coordinate and the velocity - convenient for cross‑checking with your plots `x(t)`, `y(t)`.

![Bent tube schemes](img/tube_schemes.png)

---

## Schemes reference

### `LinearAnimator` - variants 0…9

| Var | Geometry                                        | Var | Geometry                                       |
|-----|-------------------------------------------------|-----|------------------------------------------------|
| 0   | incline 30°, `O` at top, axis down‑slope        | 5   | vertical rod, `O` at bottom, axis up           |
| 1   | horizontal, `O` left, axis right                | 6   | incline 45°, `O` bottom‑right, axis up‑left    |
| 2   | horizontal, `O` right, axis left                | 7   | horizontal, `O` right, axis left               |
| 3   | incline 60°, `O` bottom‑left, axis up‑right     | 8   | horizontal rod, wall mount at `O` (left)       |
| 4   | vertical rod, `O` at top, axis down             | 9   | incline 30°, `O` top‑right, axis down‑left     |

### `TubeAnimator` - variants 0…9 (directions of `A→B` and `B→C`)

| Var | `AB` / `BC`                          | Var | `AB` / `BC`                          |
|-----|--------------------------------------|-----|--------------------------------------|
| 0   | down‑right / up‑right ("V")          | 5   | up‑right / down‑right ("roof")       |
| 1   | left / down‑left                     | 6   | right / down‑right                   |
| 2   | up‑left / left                       | 7   | left / up‑left                       |
| 3   | down‑right / right                   | 8   | down‑left / left                     |
| 4   | right / up‑right                     | 9   | up‑right / right                     |

All inclined segments are at $\alpha$ = 30° to the horizontal (the tube) or at the
angle indicated in the figure (1D schemes).

## Architecture & design patterns

| Pattern        | Where                                                             |
|----------------|-------------------------------------------------------------------|
| **Strategy**   | `IMotionLaw`, `IScalarLaw1D` - interchangeable laws of motion      |
| **Adapter**    | `FunctionalMotionLaw`, `AnalyticLaw1D`, `SampledLaw1D` - adapt functions / sample tables to the strategy interface |
| **Builder**    | `AnimatorBuilder`, `LinearAnimatorBuilder`, `TubeAnimatorBuilder`  |
| **Facade**     | `Animator`, `LinearAnimator`, `TubeAnimator` hide window, GL loader and rendering |
| **RAII**       | `Window`, shader program and GL objects owned via constructors/destructors |
| **Observer**   | `Window::onKey(...)` keyboard callbacks                            |

Rendering is done by `Renderer2D` - a batched screen‑space renderer (one dynamic VBO, one draw call per frame) with a built‑in 5×7 bitmap font for labels and the HUD. World→screen mapping (meters → pixels) is performed by the animators, so the renderer stays purely 2D and is reusable for your own visualizations.

## API cheat-sheet

| Type                    | Purpose                                                        |
|-------------------------|----------------------------------------------------------------|
| `IMotionLaw`            | 2D law: `Vec2 position(t)`, `Vec2 velocity(t)`                 |
| `FunctionalMotionLaw`   | 2D law from two `std::function<double(double)>`                |
| `IScalarLaw1D`          | 1D law: `double position(t)`                                   |
| `AnalyticLaw1D` / `SampledLaw1D` | 1D law from a function / from `{t,x}` samples          |
| `ScalarInput`           | `std::variant<function, vector<pair<double,double>>>`          |
| `Scheme1D::make(v)`     | geometry of 1D scheme variant `v` (0–9)                        |
| `TubeScheme::make(v)`   | geometry of tube scheme variant `v` (0–9)                      |
| `Renderer2D`            | `line / arrow / filledCircle / filledQuad / text` in pixels    |
| `Window`                | RAII GLFW window, `onKey` observer                             |

## Troubleshooting

- **CMake: `Target "glfw3::glfw" ... not found`.** Different GLFW installs export the target under different names. The shipped `CMakeLists.txt` accepts both (`glfw3::glfw` and `glfw`); if your system GLFW is broken for some other reason, force the bundled build: `cmake -S . -B build -DMOTIONGL_FORCE_FETCH_GLFW=ON`.
- **MinGW: old `<GL/gl.h>` misses types such as `GLchar`.** The library already ships compatibility `typedef`s in `gl_loader.hpp`; if your toolchain misses yet another type, add an analogous `typedef` there.
- **Linux: GLFW fails to configure.** Install the X11 development packages listed in [Requirements](#requirements), or install system GLFW (`sudo apt install libglfw3-dev`).
- **Nothing is rendered / context creation fails.** The library requires OpenGL 3.3 core. Very old GPUs, default VMs without 3D acceleration and remote desktop sessions without GPU support may provide only OpenGL 1.x/2.x. Enable 3D acceleration or run on real hardware.
- **The body "flies off" the axis.** Check the units of your solution (meters!) and the sign of the initial coordinate; the auto‑fitter uses the actual range of `x(t)`, so a single huge outlier sample (e.g. a numerical blow‑up) will shrink the scale - which is itself a good indicator of an unstable numerical solution.

---

Happy coding!

---

## License
Copyright (C) 2026 Stanislav Furmavnin (GitHub: saintninja). Licensed under the GNU GPL v3 or later; see [LICENSE](LICENSE). Third‑party content is not covered by this license: figures in `img/` are reproduced from course materials for reference only; the only runtime dependency, GLFW, is distributed under the zlib/libpng license.