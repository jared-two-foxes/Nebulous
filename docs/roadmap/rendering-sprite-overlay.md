# Rendering, Sprite & Overlay Roadmap

Longer-horizon ideas intentionally kept outside the active Linear implementation backlog.

## Performance and scaling

- **Sprite batching / `PT_DrawBatched`** — legacy SA-445. Consider after core migration and Beta adoption are stable.
- **StreamCompiler debug validation** — legacy SA-446.
- **Frame metrics and stream statistics** — legacy SA-447. This should precede metrics-gated optimisation work.
- **Golden-frame regression tests** — legacy SA-448.
- **Threaded/double-buffered frame pipeline** — legacy SA-449. Revisit only after correctness coverage is strong.
- **UBO migration for scene/node scopes** — legacy SA-450. Explicitly metrics-gated; may never be necessary.
- **Vector3 / UT_FLOAT3 support** — legacy SA-500. Implement when a current feature requires it rather than as speculative plumbing.

## Sprite / overlay evolution

The current **RenderStream Beta Adoption** project owns only work required to migrate existing rendering behaviour.

Potential later work preserved from the old backlog:

- Clarify/rename the screen-space overlay renderer boundary — legacy SA-472.
- Decouple SpriteAtlas texture creation from RenderSystem — legacy SA-474.
- Add a StaticGraphic `SetGraphic` mutator — legacy SA-475.
- Add GUI animation controller support — legacy SA-476.
- Broader screen-space/world-space architecture documentation beyond the enforcement work in the current adoption project — legacy SA-479.

## Promotion rule

When one of these becomes a near-term goal, review current `master`, write/update the relevant design, and create fresh implementation tickets. Do not reactivate the old ticket verbatim.
