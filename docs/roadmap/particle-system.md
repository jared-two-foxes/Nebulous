# Particle System Roadmap

The particle subsystem is not an active feature-development stream right now.

The current **RenderStream Beta Adoption** project owns only migration of today's particle rendering behaviour onto the principal render path.

Future particle capabilities preserved from the old backlog:

- Particle age semantics / removal of dead age sort code — legacy SA-462.
- Particle colour and rotation state — legacy SA-463.
- Distribution ranges for life/scale and configurable acceleration — legacy SA-464.
- Velocity-aligned billboard mode — legacy SA-465.
- Looping emitters — legacy SA-466.
- Emission shapes (point/sphere/box/cone) — legacy SA-467.
- Over-lifetime colour/scale curves — legacy SA-470.
- Measure/verify particle batching once renderer metrics exist — legacy SA-471.

Legacy SA-468/SA-469 represented an older render-integration design. That work has been consolidated into the current **RenderStream Beta Adoption** implementation project with scope limited to preserving existing behaviour.

## Promotion rule

Choose a concrete particle-system outcome first, inspect the then-current code, and generate fresh implementation tickets. These roadmap bullets are intent, not executable specifications.
