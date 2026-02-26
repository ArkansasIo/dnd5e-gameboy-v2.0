# UI API Reference

## `ui_api.h`

### Types
- `UIDockSlot`: `LEFT`, `RIGHT`, `TOP`, `BOTTOM`, `CENTER`
- `UIPanel`: panel metadata (`id`, `title`, `dock`, `visible`)
- `UIWidgetKind`: `LABEL`, `BUTTON`, `TOGGLE`, `SLIDER`, `INPUT`
- `UINotificationSeverity`: `INFO`, `WARN`, `ERROR`
- `UIWidget`: GUI widget model (`id`, `panel_id`, `label`, `kind`, state/value fields)
- `UIMenuItem`: menu path -> command mapping for GUI menu bars
- `UINotification`: transient UI toast/message with TTL
- `UIDialog`: modal/popup state (`id`, `title`, `message`)
- `UIContext`: panel/widget/menu/notification/dialog collections + active theme

### Functions
- `ui_api_init(ctx, theme_id)`
- `ui_api_add_panel(ctx, id, title, dock)`
- `ui_api_set_visible(ctx, id, visible)`
- `ui_api_find_panel(ctx, id)`
- `ui_api_apply_theme(ctx, theme_id)`
- `ui_api_add_widget(ctx, id, panel_id, label, kind)`
- `ui_api_find_widget(ctx, id)`
- `ui_api_set_widget_visible(ctx, id, visible)`
- `ui_api_set_widget_enabled(ctx, id, enabled)`
- `ui_api_set_widget_text(ctx, id, text)`
- `ui_api_add_menu_item(ctx, path, command_id)`
- `ui_api_find_menu_item(ctx, path)`
- `ui_api_find_menu_command(ctx, path)`
- `ui_api_push_notification(ctx, severity, text, ttl_frames)`
- `ui_api_tick(ctx)`
- `ui_api_open_dialog(ctx, id, title, message)`
- `ui_api_close_dialog(ctx)`
- `ui_api_save_layout(ctx, filename)`
- `ui_api_load_layout(ctx, filename)`
- `ui_api_render_console(ctx)`

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
- Widget and menu models are backend-agnostic and map cleanly to SDL/ImGui/native APIs.
- Layout persistence is provided via `ui_api_save_layout` / `ui_api_load_layout`.
- Notification/dialog state can be rendered by any backend each frame.
- Can be bound to SDL, Dear ImGui, Qt, or custom renderer.
