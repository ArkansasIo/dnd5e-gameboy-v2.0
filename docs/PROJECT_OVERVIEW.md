# Project Overview

## Repo Modules
- `src/`: Main GBDK C game runtime (menus, map, battle, save/load, audio, title flow).
- `assets/`: Source art/tables/strings.
- `tools/`: Node tools for asset conversion.
- `worldgen/`: Content generators (world, classes, monsters, story mode).
- `rpg5e/`: Node-based DND5E-style game logic framework for simulation/prototyping.

## Build Stack
- GBDK-2020 (`lcc`) for Game Boy compilation.
- GNU Make for orchestration.
- Node.js for asset and data generation.

## Current Generated Content Snapshot
- 12 kingdoms, 108 cities.
- 72 dungeons.
- Monster compendium dataset.
- Player class compendium dataset.
- OG story mode (12 acts / 50 chapters / 150 quest units).

## Direction
- Keep hardware runtime deterministic and lightweight.
- Keep large-scale content generation external (Node data pipeline).
- Bridge generated data into runtime-friendly formats incrementally.
