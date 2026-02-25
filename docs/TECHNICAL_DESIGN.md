# Technical Design

## Runtime Architecture
- Engine Layer: rendering, input, memory, timing (`core.*`).
- Game Layer: menu, map, encounter, battle, progression.
- Data Layer: generated/static data loaded into banked assets.

## Data Architecture
- Source data generated via `worldgen/src/*.js`.
- JSON artifacts stored under `worldgen/generated/`.
- Runtime migration path: convert JSON to compact C/binary tables for ROM inclusion.

## Build Pipeline
1. `make assets` runs Node tools (`strings2c`, `tables2c`, `png2bin`).
2. GBDK compiles C sources and banked data.
3. ROM output: `LabyrinthOfTheDragon.gbc`.

## Integration Strategy (Generated Data -> Runtime)
- Phase 1: Keep generated JSON as design-time truth.
- Phase 2: Add converter scripts to emit C headers/source tables.
- Phase 3: Runtime chapter/quest loading by act/chapter IDs.

## Performance Constraints
- ROM bank limits and tile budgets.
- Keep string and table data compact.
- Prefer deterministic turn logic and compact state structs.

## Reliability Notes
- Avoid non-portable C library features unsupported by SDCC target.
- Avoid path-space-sensitive shebang invocations in Makefile tool commands.
