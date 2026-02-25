# Content Pipeline

## Authoring Sources
- Story generator: `worldgen/src/generate_story_mode_og.js`
- World generator: `worldgen/src/generate_world.js`
- Class generator: `worldgen/src/generate_player_classes_5e.js`
- Monster generator: `worldgen/src/generate_monster_compendium.js`

## Commands
- `npm run worldgen`
- `npm run worldgen:story`
- `npm run worldgen:classes`
- `npm run worldgen:monsters`
- `make assets`
- `make`

## Data Promotion Path
1. Generate JSON in `worldgen/generated`.
2. Validate structure and counts.
3. Convert to runtime C/binary assets.
4. Build ROM and run emulator checks.

## Failure Modes
- Path-space issues for shebang scripts.
- Missing GBDK include path for IntelliSense.
- SDCC incompatibilities with advanced libc calls.

## Release Hygiene
- Lock generator versions per release tag.
- Regenerate content deterministically before release candidate.
- Commit generated assets with manifest checks.
