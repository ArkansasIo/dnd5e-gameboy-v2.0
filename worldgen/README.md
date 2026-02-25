# Worldgen Module

This folder provides source code and generated data files for a large DND5E-style world scaffold.

## Source Files
- `src/generate_world.js`: Generates all world data JSON files.
- `src/generate_monster_compendium.js`: Generates a large DND5E-style monster catalog.

## Generated Files
- `generated/world.json`
- `generated/biomes.json`
- `generated/races.json`
- `generated/classes.json`
- `generated/creatures.json`
- `generated/zone_maps.json`
- `generated/kingdom_lore.json`
- `generated/npcs.json`
- `generated/quests.json`
- `generated/encounter_tables.json`
- `generated/dungeons.json`
- `generated/monster_compendium.json`
- `generated/lore/*.md` (one markdown lore file per kingdom)

## What It Generates
- Continents and countries.
- 12 kingdoms total.
- 9 cities per kingdom.
- 2 towns per city.
- 3 villages per city.
- Surface zone maps and underground zones per kingdom.
- Biome class/subclass/type/subtype taxonomy.
- Races and race subtypes.
- Classes and subclasses.
- Monsters, creatures, and animals with types/subtypes and biome tags.
- Kingdom lore files (gazetteer style).
- NPC datasets for cities.
- Quest datasets tied to kingdom zones.
- Encounter tables by biome type and danger tier (1-5).
- 72 dungeons total (6 per kingdom) with themes, tiers, bosses, and zone links.
- Large monster compendium dataset for content production.

## Run
```powershell
npm run worldgen
```

Monster compendium only:

```powershell
npm run worldgen:monsters
```

or

```powershell
node worldgen/src/generate_world.js
```
