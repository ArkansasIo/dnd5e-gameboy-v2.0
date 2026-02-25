# IDE Editor Feature Set

This editor now includes a functional core for game-source, asset, and data editing workflows.

## Implemented Core Features
- Project asset database (`project_db.*`)
  - Asset registration, update, remove, dirty tracking.
- Tilemap editing tools (`map_editor.*`)
  - Paint tile, rectangle fill, flood fill, CSV export.
- Undo/redo command stack (`command_stack.*`)
  - Command push, undo pop, redo pop.
- Asset tools (`asset_tools.*`)
  - File copy helper and manifest generation.
- Game engine tools (`game_engine_tools.*`)
  - Encounter budget validation helper.
- App integration (`ide_editor.*`)
  - Startup seeding of menus/sidebar/project assets.
  - Autosave of tilemap and generated manifest on exit.
  - Command dispatch + hotkey routing.
  - Theme switching and UI panel layout management.
  - Project + settings persistence (`autosave_project.db`, `editor_settings.ini`).
  - Source workspace scan/open/save/compile (`source_tools.*`) with active-file tracking.
  - Source line operations: insert stub, delete cursor line, next `#include` search.
  - Interactive command palette (toggle/filter/select/execute) with keyboard navigation.
  - Center viewport now renders a live Game Boy-style 20x18 preview (tiles + actors + triggers).
  - Runtime game-logic simulation module (`engine_runtime.*`) with quests/entities/combat tick.

## Unreal-Style Editor Framework Added
- Settings/options system (`editor_settings.*`)
- Theme engine (`editor_theme.*`)
- Tool registry (`editor_tools.*`)
- GUI/UI API abstraction with docked panels (`ui_api.*`)
- Command registry (`command_registry.*`)
- Menu tree with submenus and command IDs (path-based hierarchy)

## Existing Editor Modules
- Window shell: `window.*`
- Menu model: `menu.*`
- Sidebar model: `sidebar.*`
- File buffer editing: `editor_file.*`
- Syntax tokenization: `syntax_highlight.*`
- Blueprint graph model: `blueprint.*`

## Build
`build.bat` compiles all C files in `ide_editor`.

### New Hotkeys
- `F3`: Find next `#include` in active source file
- `F4`: Save active source file
- `F7`: Scan source folder
- `F8`: Compile active source
- `F9`: Toggle command palette
- `I`: Runtime tick
- `F`: Player runtime attack
- `[` and `]`: Quest next/progress

## Planning
- Full roadmap and missing systems checklist:
  - `IDE_FULL_TODO.md`

## Next Expansion Targets
- Real UI backend (SDL/ImGui/native) for visible tool interaction.
- Live tileset preview and stamp brushes.
- Blueprint links/validation and node execution previews.
- DB persistence (JSON/SQLite) and project load/save.
