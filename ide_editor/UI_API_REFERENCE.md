# UI API Reference

## `ui_api.h`

### Types
- `UIDockSlot`: `LEFT`, `RIGHT`, `TOP`, `BOTTOM`, `CENTER`
- `UIPanel`: panel metadata (`id`, `title`, `dock`, `visible`)
- `UIContext`: panel collection + active theme

### Functions
- `ui_api_init(ctx, theme_id)`
- `ui_api_add_panel(ctx, id, title, dock)`
- `ui_api_set_visible(ctx, id, visible)`
- `ui_api_find_panel(ctx, id)`
- `ui_api_apply_theme(ctx, theme_id)`

## Default Panels
- World Hierarchy
- Asset Outliner
- 2D Viewport
- Details
- Content Browser
- Output Log
- Blueprint Graph
- Perf/Memory

## Integration Notes
- Docking and panel state are backend-agnostic.
- Can be bound to SDL, Dear ImGui, Qt, or custom renderer.
