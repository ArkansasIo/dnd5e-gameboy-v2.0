# IDE Feature Pack (Scaffold)

This folder contains a full source-file scaffold for the requested systems:
- Visual game builder and debugger hooks
- Mixed scene types (adventure/platformer/shooter)
- Tile/sprite/UI/font/SGb asset systems
- Music editor interfaces (piano roll/tracker)
- Event/dialogue/inventory/quest/actor-collision logic layers
- Plugin manager + extension API
- Build/export (ROM/web) + runtime tuning + camera systems

Status:
- APIs and module boundaries are implemented as compile-safe scaffolds.
- Each module has init/validate/tick entry points ready for full logic expansion.

Next implementation pass:
1. Wire modules into ide_editor.c runtime lifecycle.
2. Replace stub tick/validate behavior with real data models.
3. Add serialization for per-module configs.
4. Add integration tests for command-to-feature behavior.
