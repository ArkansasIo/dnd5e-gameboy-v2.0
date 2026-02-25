# Tools, Tables, and Source Editing Docs

## Project Database Table

### `DBAssetRecord`
- `id` (int): stable asset id.
- `type` (enum): tileset/tilemap/sprite/sound/script/data/other.
- `name` (string): editor display name.
- `path` (string): project-relative or absolute path.
- `tag` (string): category tag (`zone-map`, `story`, etc).
- `dirty` (bool): modified but not synchronized.

### `ProjectDB`
- `assets[4096]`
- `asset_count`
- `next_id`

## Editing Operation Table

### Tilemap operations
- `tilemap_set(x,y,tile)`
- `tilemap_fill_rect(x0,y0,x1,y1,tile)`
- `tilemap_bucket_fill(x,y,tile)`
- `tilemap_save_csv(file)`

### Command operations
- `command_stack_push(cmd)`
- `command_stack_pop_undo()`
- `command_stack_pop_redo()`

## Engine Tooling
- Encounter budget guard:
  - Input: chapter level range + enemy CR total.
  - Output: pass/fail + reason string.

## Source Files to Extend
- `ide_editor.c`: lifecycle, integration, automation hooks.
- `project_db.c`: DB table operations and integrity checks.
- `map_editor.c`: map edit logic and exporters.
- `asset_tools.c`: import pipeline and manifesting.
- `game_engine_tools.c`: balancing and validation rules.
