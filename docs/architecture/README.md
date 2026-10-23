# Nebulous architecture model

Open [Nebulous.mdj](Nebulous.mdj) in StarUML. The project contains three editable UML diagrams:

| Diagram | Purpose |
| --- | --- |
| Engine packages and dependencies | Bazel library dependencies and the GL plugin boundary |
| Rendering abstraction | RenderSystem, its OpenGL implementation, resource factories, and stream/resources |
| Scene structure | Graph, nodes, objects, slots, camera, and stream compiler |

The diagrams describe selected relationships, not every header or call. Classes are shared model elements across diagrams. Methods and attributes are selective summaries; inspect the linked source before treating a signature or relationship as exhaustive. The Audio package represents the current deferred header-only Bazel target. GL is compiled against Alpha and loaded through `CreateRenderSystem` at runtime.

This snapshot was traced from [master at `b755cf4531da`](https://github.com/jared-two-foxes/Nebulous/tree/b755cf4531dabecc5c21812a4c7f2d85ec9a8b41), especially:

- `Source/Nebulae/{Common,Alpha,Audio,Beta}/BUILD.bazel` and `RenderSystems/GL/BUILD.bazel`
- `Source/Nebulae/Alpha/Alpha.cpp`, `RenderSystem/RenderSystem.h`, `Resource/ResourceFactory.h`, and `RenderStream/RenderStream.h`
- `RenderSystems/GL/Includes/RenderSystem_OGL.h` and `RenderSystems/GL/main.cpp`
- `Source/Nebulae/Beta/Scene/{SceneGraph,SceneNode,SceneObject}.{h,cpp}`
- `Source/Nebulae/Beta/RenderQueue/StreamCompiler.{h,cpp}`

SceneGraph owns and deletes nodes; nodes own their attached SceneObjects; nodes also link to their parent and children. RenderSlot values belong to SceneObject. The rendering model depicts the GL implementation as an inheritance relationship and the stream as a dependency. The package view shows compile-time dependencies; the GL plugin note describes dynamic loading.

Maintain the StarUML project as curated architecture documentation. Update the relevant model element and diagram after a design change, retaining manual arrangement and annotations. Avoid regenerating the whole project from declarations.
