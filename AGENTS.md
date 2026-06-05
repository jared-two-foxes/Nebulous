# Nebulous (Nebulae Engine) — Agent Guide

## One-liner

C++ game engine built with **Premake5** (not CMake). 4 static libs + plugin DLLs + Google Test unit tests. Last active ~2017, VS 2017 era toolchain.

## Build system

- **Root entrypoint**: `premake5.lua` loads `workspace.lua` which defines all projects and their inter-dependencies.
- **Required tool**: [Premake5](https://premake.github.io/) (v5).
- **Generate Visual Studio solution**:
  ```
  premake5 vs2019
  ```
- **Custom output directory** (avoids cluttering root):
  ```
  premake5 --outdir=_build vs2019
  ```
- **Configurations**: `Debug` / `Release`, x86_64 only.
- **Key workspace defines**: `PREMAKE5`, `NEBULAE_INCLUDES_PNG`.
- **Compiler flags**: `/EHsc` /MP` (MSVC multi-process compilation).
- **`workspace.lua`** contains **hardcoded absolute paths** (`C:/Develop/Nebulous/...`). These must be updated if the repo moves — they are not relative to the script.
- External dependency config lives in `libraries.lua` files alongside each subproject (tests, audio, rendersystems). The root `premake5.lua` also expects `libraries.lua` either in the repo root or at the path in `%USERPROFILE%`. Currently no root-level `libraries.lua` exists — running `premake5` will fail without one.

## Project architecture

| Directory | Type | Purpose |
|---|---|---|
| `Source/Nebulae/Common/` | StaticLib | Foundation: math, filesystem, logging, windowing, serialization, platform abstraction |
| `Source/Nebulae/Alpha/` | StaticLib | Render abstraction: buffers, textures, shaders, input layouts, plugin accessor |
| `Source/Nebulae/Alpha/Audio/` | StaticLib | Audio subsystem: OGG/WAV decoders, OpenAL backend |
| `Source/Nebulae/Beta/` | StaticLib | Application framework: GUI system, materials, sprites, animations, `StateStack`, camera |
| `RenderSystems/GL/` | SharedLib (DLL) | OpenGL 3 render plugin, platform Win32 |
| `RenderSystems/GLES2/` | SharedLib (DLL) | OpenGL ES 2 render plugin — Win32 + iOS (EAGL) |
| `Samples/*/` | WindowedApp | Example apps using engine |
| `tests/` | ConsoleApp | Google Test unit tests |

**Dependency chain**: `Common → Alpha → Beta` (plus Audio as a sibling to Alpha).

RenderSystems are loaded **at runtime via DLL** — `Alpha::CreateRenderSystem()` loads plugin by name:

```cpp
nebulous_gl3.dll       // debug: nebulous_gl3_d.dll
nebulous_gles_2_0.dll
nebulous_vulcan.dll
```

## Tests

- Google Test, linked as **shared library** (`GTEST_LINKED_AS_SHARED_LIBRARY` defined).
- Single test project at `tests/` that links all engine libs + gmock.
- Test categories by subdirectory: `Common/`, `Alpha/`, `Beta/`, `Audio/`.
- Test runner entry: `tests/main.cpp` — vanilla `RUN_ALL_TESTS()`.
- Run tests from the generated VS solution (Debug target `nebulous_tests`).

## Key external dependencies

All linked via `libraries.lua` config files:

- **boost** (header + lib)
- **brofiler**
- **jsoncpp**
- **libpng**
- **freetype**
- **utf8** (cpputf8)
- **openal-soft** (Audio)
- **googletest** + **gmock** (tests)
- **opengl32.lib** (GL render system)

## Conventions and quirks

- `Nebulae::` namespace throughout.
- Platform detection via `NE_PLATFORM_WIN32` / `NE_PLATFORM_APPLE_IOS` etc. — not the standard preprocessor checks exposed by most libraries.
- Plugin API versioning via `NE_PLUGIN_API_VERSION` macro — ABI mismatch throws `std::runtime_error`.
- Default root filesystem device is `"disk"` (plain file access). Toggle `USE_ZIPDEVICE_AS_DEFAULT_ROOT` in `Common/Config.h` for zipped archives.
- Library naming convention on Windows: debug libs get `d` suffix (e.g., `nebulous_commond.lib`).
- `Backlog` file at root tracks known TODOs and past work — consult before duplicating effort.
