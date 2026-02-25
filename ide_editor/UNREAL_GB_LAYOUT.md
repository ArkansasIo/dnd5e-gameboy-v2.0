# Unreal + GB Studio Layout Preset

This preset provides a UE5-style editor shell adapted for 2D GB workflows.

## Top Menu Bar

- `File`
  - New/Open/Save/Save All
  - Import (Asset, Map Data)
  - Export (Manifest, Build Package)
- `Edit`
  - Undo/Redo
  - Duplicate/Delete Selection
  - Editor Preferences
- `View`
  - Theme presets
  - Panel visibility shortcuts
- `Tools`
  - Terrain tools
  - Object transform tools
  - Debug tools
- `Blueprint`
  - New/Compile/Validate graph
- `Build`
  - Build/Run/Validate
- `GB Studio`
  - Scenes, Actors, Triggers, Scripts, Play, Build, Project
- `Window`
  - Load/Save/Reset layout preset
- `Help`
  - Quick Start, Controls, About

## Panel Zones

- Left dock:
  - World Hierarchy
  - Asset Outliner
  - Scene Tree
  - Actor List
- Center dock:
  - 2D Viewport
  - Game Preview
  - Mini Map
  - Blueprint Graph
- Right dock:
  - Details
  - Inspector
  - Tile Palette
  - Perf/Memory
- Bottom dock:
  - Content Browser
  - Output Log
  - Timeline
  - Command Palette

## Toolbar and Tool Shelf

- Global toolbar:
  - New, Open, Save, Play, Stop, Build, Validate
- Tool shelf:
  - Select, Move, Rotate, Scale
  - Paint, Fill, Erase
  - Actor, Trigger, Script
- Viewport toggles:
  - Show Grid, Snap To Tile, Show Collisions

## Example Layout (ASCII)

```
+--------------------------------------------------------------------------------+
| [Menu Bar: File | Edit | View | Tools | Build | Blueprint | GB Studio | Window | Help] |
+--------------------------------------------------------------------------------+
| [Toolbar: New | Open | Save | Play | Stop | Build | Validate]                  |
+---+-----------------------------+-----------------------------+---+
|TS |                             |                             | R |
| o |                             |                             | i |
| o |        2D Viewport          |         Details/Inspector    | g |
| l |        Game Preview         |         Tile Palette         | h |
| S |        Blueprint Graph      |         Profiler             | t |
| h |        Mini Map             |                             |   |
| e |                             |                             |   |
| l |                             |                             |   |
| f |                             |                             |   |
+---+-----------------------------+-----------------------------+---+
| [Content Browser | Output Log | Timeline | Command Palette]        |
+-------------------------------------------------------------------+
```

## Widget/Window Examples

- **Settings Window:**
  - Theme, Font Size, Key Bindings, Autosave, Layout Presets
- **Asset Browser:**
  - File tree, search, preview, import/export
- **Code Editor:**
  - Syntax highlighting, tabs, find/replace
- **Blueprint Editor:**
  - Node graph, drag-and-drop, compile/validate
- **Build Output:**
  - Console log, error/warning list
- **Help/About:**
  - Quick start, controls, about info

## Notes
- All panels and widgets are modular and can be shown/hidden or rearranged.
- The UI API supports adding/removing panels, widgets, and menu items programmatically.
- Layout is saved to `editor_ui_layout.ini` and restored on launch.

---
For further customization, edit the UI API or this layout guide.
