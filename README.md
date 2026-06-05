# Nebulous (Nebulae Engine)

A C++ game engine built with **Bazel 7+** (bzlmod/BCR). Originally developed around 2017 with a Premake5/VS2017 toolchain; now fully migrated to Bazel on Windows MSVC.

## Getting Started

### Prerequisites

- **Bazelisk** (recommended) or **Bazel 7+** -- [Install Bazelisk](https://github.com/bazelbuild/bazelisk)
- **MSVC Build Tools** -- Visual Studio 2022 or Build Tools for Visual Studio 2022
- **Windows SDK** (10.x or later)

### Build Everything

```
bazel build //...
```

### Run All Tests

```
bazel test //... --test_output=errors
```

### Common Commands

| Command | Description |
|---------|-------------|
| bazel build //... | Build all targets |
| bazel test //... --test_output=errors | Run all tests |
| bazel test //tests:common_tests | Run Common library tests |
| bazel test //tests:alpha_tests | Run Alpha library tests |
| bazel test //tests:beta_tests | Run Beta library tests |
| bazel build //Source/Nebulae/Common:common | Build Common static lib |
| bazel build //Source/Nebulae/Alpha:alpha | Build Alpha static lib |
| bazel build //Source/Nebulae/Beta:beta | Build Beta static lib |
| bazel build //RenderSystems/GL:nebulous_gl3 | Build GL render plugin DLL |
| bazel build --config=debug //... | Debug build |
| bazel build --config=opt //... | Release (optimized) build |
| bazel clean && bazel build //... | Clean + full rebuild |

## Project Structure

| Directory | Type | Purpose |
|-----------|------|---------|
| Source/Nebulae/Common/ | Static library | Foundation: math, filesystem, logging, windowing, serialization |
| Source/Nebulae/Alpha/ | Static library | Rendering abstraction: buffers, textures, shaders |
| Source/Nebulae/Alpha/Audio/ | Static library | Audio subsystem (deferred -- no openal-soft BCR module) |
| Source/Nebulae/Beta/ | Static library | Application framework: GUI, materials, sprites, animations |
| RenderSystems/GL/ | Shared library (DLL) | OpenGL 3 render plugin |
| RenderSystems/GLES2/ | Shared library (DLL) | OpenGL ES 2 render plugin (deferred -- needs ANGLE/Mali SDK) |
| Samples/ | Applications | Example apps (not yet Bazel-ified) |
| tests/ | Test suite | Google Test unit tests (51 tests, all passing) |

**Dependency chain**: Common -> Alpha -> Beta (Audio parallels Alpha)

RenderSystems are loaded at runtime via DLL: Alpha::CreateRenderSystem() loads the plugin by name.

## Test Status

All **51** tests pass:

- common_tests: 16/16 [PASS]
- alpha_tests: 9/9 [PASS]
- beta_tests: 26/26 active + 6 DISABLED_ (desktop-dependent) [PASS]

## Dependencies

All external dependencies are managed via **BCR (Bazel Central Registry)** modules in MODULE.bazel:

- **Boost** 1.90.0 -- any, filesystem, lexical_cast, signals2, format, xpressive, assign, chrono, thread
- **libpng** 1.6.37 (with Windows-compatible patch)
- **freetype** 2.13.0
- **jsoncpp** 1.9.6
- **zlib** (transitive)
- **googletest** (gtest + gmock)
- **opengl32.lib**, **user32.lib**, **gdi32.lib** (system/Win32 SDK)

Header-only stubs in third_party/:
- **brofiler** -- profiler header stubs
- **utfcpp** -- UTF-8 library

## Building the GL Plugin

The OpenGL 3 render system builds as a DLL:
```
bazel build //RenderSystems/GL:nebulous_gl3
```
Output: bazel-bin/RenderSystems/GL/nebulous_gl3.dll

## Conventions

- Nebulae:: namespace throughout.
- Platform detection: NE_PLATFORM_WIN32 / NE_PLATFORM_APPLE_IOS etc.
- Plugin ABI versioning via NE_PLUGIN_API_VERSION macro.
- C++14 per-library; googletest uses C++17.
- Debug libraries get a d suffix (e.g., nebulous_commond.lib).

## License

See Backlog for known TODOs and past work.
