# GUI Roadmap

The previous Nebulous Linear project contained a full GUI modernisation backlog even though GUI work was not the current implementation focus.

Ideas preserved from legacy SA-480–SA-493:

- Stable widget IDs and lookup.
- GuiManager lifecycle/clear behaviour.
- Mouse repeat restoration.
- Widget rotation deserialisation.
- Fix pressed/unpressed graphic ownership API.
- Relative mouse movement.
- Double-click dispatch.
- FontManager loading/ownership/cache correctness.
- Remove dead GuiManager RenderSystem dependency.
- Improve DepthList move operations.
- Resolve or remove the dead Layout abstraction.
- Visible keyboard focus ring.
- LayoutAssistant resolution/aspect-ratio behaviour.
- Replace `boost::signals2` callback usage where appropriate.

These should be planned as a finite **GUI Modernisation** feature/system when GUI becomes current work. At that point, re-audit the current GUI code and create fresh tickets rather than reviving old numbered tasks.
