# Nebulous (Nebulae Engine) -- Agent Guide

## One-liner

C++ game engine built with **Bazel 7+** (bzlmod/BCR). 4 static libs + plugin DLLs + Google Test unit tests. Windows MSVC toolchain.

## Toolchain

| Command | Value |
|---|---|
| `BUILD_CMD` | `bazel build //...` |
| `TEST_CMD` | `bazel test //... --test_output=errors` |
| `FMT_CHECK_CMD` | `bazel run //tools/lint:format_check` |
| `FMT_FIX_CMD` | `bazel run //tools/lint:format_fix` |
| `LINT_CMD` | `bazel run //tools/lint:clang_tidy` |
| `TYPECHECK_CMD` | `_(none)_` — subsumed by BUILD_CMD |
| `IWYU_CMD` | `bazel run //tools/lint:include_what_you_use` |
| `GIT_WORKFLOW` | `trunk-based` |

## Build system

- **Root entrypoint**: MODULE.bazel defines all external dependencies via BCR modules.
- **Required tool**: [Bazelisk](https://github.com/bazelbuild/bazelisk) or [Bazel 7+](https://bazel.build/).
- **Build everything**:
  ```
  bazel build //...
  ```
- **Run all tests**:
  ```
  bazel test //... --test_output=errors
  ```
- **Run specific test suite**:
  ```
  bazel test //tests:common_tests --test_output=all
  ```
- **Build a specific target**:
  ```
  bazel build //Source/Nebulae/Common:common
  ```
- **Build the GL render plugin DLL**:
  ```
  bazel build //RenderSystems/GL:nebulous_gl3
  ```
- **Build with debug config**:
  ```
  bazel build --config=debug //...
  ```
- **Build with optimizations**:
  ```
  bazel build --config=opt //...
  ```
- **Clean build**:
  ```
  bazel clean && bazel build //...
  ```
- **Configurations**: Debug (default), Release (--config=opt), x86_64 only.
- **Key workspace defines**: NEBULAE_INCLUDES_PNG (set in Alpha BUILD.bazel).
- **Compiler flags**: /EHsc, /MP, /W4 configured in .bazelrc.
- **Output directory**: bazel-bin/, bazel-out/, bazel-testlogs/ (symlinks in repo root, actual artifacts in %USERPROFILE%/.bazel/).
- **Disk cache**: %LOCALAPPDATA%/Bazel/cache for faster rebuilds.

## Project architecture

| Directory | Type | Purpose |
|---|---|---|
| Source/Nebulae/Common/ | StaticLib | Foundation: math, filesystem, logging, windowing, serialization, platform abstraction |
| Source/Nebulae/Alpha/ | StaticLib | Render abstraction: buffers, textures, shaders, input layouts, plugin accessor |
| Source/Nebulae/Alpha/Audio/ | StaticLib | Audio subsystem: OGG/WAV decoders, OpenAL backend (deferred, no BCR module) |
| Source/Nebulae/Beta/ | StaticLib | Application framework: GUI system, materials, sprites, animations, StateStack, camera |
| RenderSystems/GL/ | SharedLib (DLL) | OpenGL 3 render plugin, platform Win32 |
| RenderSystems/GLES2/ | SharedLib (DLL) | OpenGL ES 2 render plugin -- needs ANGLE/Mali SDK (deferred) |
| Samples/*/ | WindowedApp | Example apps using engine (Bazel-ified) |
| tests/ | ConsoleApp | Google Test unit tests |

**Dependency chain**: Common -> Alpha -> Beta (plus Audio as a sibling to Alpha).

RenderSystems are loaded **at runtime via DLL** -- Alpha::CreateRenderSystem() loads plugin by name:

```cpp
nebulous_gl3.dll       // debug: nebulous_gl3_d.dll
nebulous_gles_2_0.dll
nebulous_vulcan.dll
```

## Tests

- Google Test, linked as **shared library** (GTEST_LINKED_AS_SHARED_LIBRARY defined).
- Three test suites: common_tests, alpha_tests, beta_tests.
- Test categories by subdirectory: Common/, Alpha/, Beta/, Audio/ (pending).
- Test runner entry: tests/main.cpp -- runfiles-staging for Bazel on Windows.
- Run all tests:
  ```
  bazel test //tests:all_tests --test_output=errors
  ```
- Test assets are staged from RUNFILES_MANIFEST_FILE at runtime (Bazel manifest-only runfiles on Windows).

## Key external dependencies

All managed via BCR modules in MODULE.bazel:

- **boost** (any, filesystem, lexical_cast, signals2, format, xpressive, assign, chrono, thread) -- v1.90.0
- **libpng** -- v1.6.37 (with patches/libpng_no_bash.patch for Windows)
- **freetype** -- v2.13.0
- **jsoncpp** -- v1.9.6
- **zlib** (transitive dep)
- **googletest** (gtest + gmock) -- latest
- **opengl32.lib** (system, GL render system)
- **user32.lib**, **gdi32.lib** (system, windowing)
- **utf8** / cpputf8 (header-only in third_party/utfcpp/)

## Conventions and quirks

- Nebulae:: namespace throughout.
- Platform detection via NE_PLATFORM_WIN32 / NE_PLATFORM_APPLE_IOS etc. -- not the standard preprocessor checks exposed by most libraries.
- Plugin API versioning via NE_PLUGIN_API_VERSION macro -- ABI mismatch throws std::runtime_error.
- Default root filesystem device is 'disk' (plain file access). Toggle USE_ZIPDEVICE_AS_DEFAULT_ROOT in Common/Config.h for zipped archives.
- Library naming convention on Windows: debug libs get d suffix (e.g., nebulous_commond.lib).
- Backlog file at root tracks known TODOs and past work -- consult before duplicating effort.
- Bazel Windows manifest-only runfiles: tests re-stage assets to $TEST_TMPDIR at startup.
