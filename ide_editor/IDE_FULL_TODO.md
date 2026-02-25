# IDE Full Upgrade TODO

## Implemented In This Pass
- Runtime simulation module added (`engine_runtime.*`)
  - entities, hostiles, player HP, quest progression, runtime tick
  - runtime commands integrated into IDE menu/hotkeys/buttons
- Center viewport Game Boy-style preview (20x18) already active
- Source workspace + command palette flows already active

## Missing Core Editor Features
- Real code editor widget
  - text selection, caret movement, multiline editing, clipboard
  - syntax color rendering and diagnostics panel
- Project explorer tree
  - folders/files with create/rename/delete/move
  - click-to-open in code editor
- Asset pipeline UI
  - import spritesheets/tilesets/audio
  - metadata inspector and validation warnings
- Scene/map workflow parity with GB Studio
  - scene list panel, actor list panel, trigger list panel
  - drag/drop placement in viewport
- Build/deploy pipeline
  - configurable target profiles (debug/release)
  - build output parsing with clickable errors
- Integrated emulator plugin
  - run/pause/step/reset
  - ROM auto-reload when build completes

## Missing Game Engine Logic Systems
- Combat system
  - initiative/turn order timeline
  - damage types, resistances, status effects
- Quest/event system
  - conditional objectives, branching outcomes
  - persistent quest state in project DB
- Inventory/equipment
  - item database, loadout slots, stat modifiers
- AI behavior toolkit
  - patrol/chase/flee states
  - encounter region behavior hooks
- Save/load runtime state
  - snapshot slots and rollback safety

## Theme and UX TODO
- Theme presets to add
  - `GB Studio Light` (high readability for long editing sessions)
  - `CRT Green` (retro handheld style)
  - `High Contrast Pro` (accessibility)
  - `Blueprint Steel` (current Unreal-like extended variant)
- Theme engine upgrades
  - spacing scale tokens and icon color tokens
  - focus/hover/pressed state colors per widget class
  - warning/error/success semantic palette
- UX polish
  - persistent dock layout profiles
  - keyboard shortcut editor UI
  - command palette fuzzy matching and category grouping

## Documentation To Add
- `EDITOR_USER_GUIDE.md`
  - install/build/run + daily workflow
- `EDITOR_SHORTCUTS.md`
  - full key map and action table
- `RUNTIME_LOGIC_REFERENCE.md`
  - simulation model and command behaviors
- `PLUGIN_API.md`
  - emulator/plugin integration interface

## QA and Stability
- Unit tests for:
  - command registry and command dispatch
  - source workspace scan/open/save/compile
  - runtime quest progression and combat edge cases
- Crash diagnostics
  - session replay log with last command trail
  - startup safe mode (disable plugins and custom layout)

## Priority Order
1. Code editor widget + diagnostics panel
2. Project explorer + click-to-open files
3. Emulator plugin and build/error integration
4. Scene/actor/trigger visual editing tools
5. Runtime systems persistence and test coverage
