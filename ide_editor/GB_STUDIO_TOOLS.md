# GB Studio Tools Integration

This editor now includes a GB Studio-style toolset for scene, actor, trigger, script, and runtime gameplay logic workflows.

## Menus and Submenus

- `GB Studio/Scenes/New Scene` -> `gb.scene.new`
- `GB Studio/Actors/Add NPC` -> `gb.actor.add`
- `GB Studio/Actors/Add Hostile` -> `gb.actor.add_hostile`
- `GB Studio/Triggers/Add Trigger` -> `gb.trigger.add`
- `GB Studio/Scripts/New Script Event` -> `gb.script.add`
- `GB Studio/Scripts/New Reward Script` -> `gb.script.add.reward`
- `GB Studio/Scripts/New Damage Script` -> `gb.script.add.damage`
- `GB Studio/Scripts/Run Last Script Event` -> `gb.script.run_last`
- `GB Studio/Scenes/Next Scene` -> `gb.scene.next`
- `GB Studio/Scenes/Previous Scene` -> `gb.scene.prev`
- `GB Studio/Build/Validate Active Scene` -> `gb.validate.scene`
- `GB Studio/Build/Validate All Scenes` -> `gb.validate.all`
- `GB Studio/Play/Single Tick` -> `gb.play.tick`
- `GB Studio/Play/Toggle Play Mode` -> `gb.play.toggle`
- `GB Studio/Project/Save` -> `gb.project.save`
- `GB Studio/Project/Load` -> `gb.project.load`

## Hotkeys

- `G` create scene
- `H` add actor to active scene
- `J` add trigger
- `K` run last script event
- `V` validate active scene
- `B` validate all scenes
- `N` next scene
- `X` previous scene
- `L` toggle play mode
- `Y` save GB Studio project
- `Z` load GB Studio project

## Implemented Features

- Scene authoring
  - create named scenes with size and background tileset id
- Actor placement
  - actor type/name/sprite id/position/speed/hostility
- Trigger authoring
  - trigger rectangle linked to script event id
- Script events
  - `SET_FLAG`, `ADD_GOLD`, `DAMAGE_PLAYER`, `HEAL_PLAYER`, `TELEPORT`, `NOP`
- Validation
  - scene actor/trigger budget checks
- Persistence
  - save/load GB Studio project state (`gbstudio_project.db`)
- Runtime logic
  - world tick counter
  - passive HP regeneration
  - simple hostile actor patrol motion on active scene

## Core Source Files

- `gb_studio_tools.h`
- `gb_studio_tools.c`
- `ide_editor.c` (integration: command registration, menu wiring, hotkeys, runtime tick)
- `ide_editor.h` (project state field: `GBStudioProject gb_project`)
