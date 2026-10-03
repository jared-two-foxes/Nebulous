# Samples & Developer Experience Roadmap

The old backlog mixed sample-app restructuring with the renderer migration. Samples are useful, but they are not part of the current execution plan.

Preserved goals from legacy SA-451 and SA-494–SA-499:

- Organise samples so they clearly teach Common / Alpha / Beta layers.
- Keep a minimal Alpha rendering sample.
- Provide a richer Alpha RenderStream sample demonstrating textures, index buffers, multiple draws, and relevant uniform types.
- Remove accidental Beta dependencies from Alpha-focused samples.
- Improve sample READMEs and build coverage.
- Upgrade the Cube/sample scene when it serves a current teaching or validation need.

SA-451's original purpose — proving the Alpha-only stream during the temporary broken scene-rendering window — has expired because the SceneGraph/StreamCompiler path is now implemented.

If an Alpha RenderStream sample is wanted later, design it from the current API rather than implementing SA-451 literally.
