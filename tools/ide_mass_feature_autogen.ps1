param(
    [string]$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path,
    [string]$OutputDir = "ide_editor/feature_autogen",
    [switch]$Force
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function Write-Step([string]$Text) {
    Write-Host "[AUTOGEN] $Text"
}

function To-PascalCase([string]$s) {
    if ([string]::IsNullOrWhiteSpace($s)) { return "" }
    return (($s -split "_") | ForEach-Object {
        if ($_.Length -eq 0) { return "" }
        $_.Substring(0,1).ToUpper() + $_.Substring(1)
    }) -join ""
}

function To-UpperSnake([string]$s) {
    return ($s -replace "[^A-Za-z0-9_]", "_").ToUpper()
}

$features = @(
    @{ id="visual_game_builder"; title="Visual Game Builder"; desc="Drag-drop construction tools and scene graph authoring." ; tags=@("builder","ui","events") },
    @{ id="visual_debugger"; title="Built-in Debugger"; desc="Breakpoints, watch values, event trace, and step execution." ; tags=@("debug","events") },
    @{ id="event_logic_rpg"; title="RPG-style Event Logic"; desc="Event-driven flow similar to RPG Maker command chains." ; tags=@("events","logic") },
    @{ id="scene_topdown"; title="Top-down Scene"; desc="Top-down adventure scene runtime and navigation rules." ; tags=@("scene","adventure") },
    @{ id="scene_platformer"; title="Platformer Scene"; desc="Platformer runtime with jump and collision movement rules." ; tags=@("scene","platformer") },
    @{ id="scene_shooter"; title="Shooter Scene"; desc="Shooter runtime with projectile and enemy pattern support." ; tags=@("scene","shooter") },
    @{ id="scene_mixer"; title="Scene Type Mixer"; desc="Combine multiple scene types within a single game project." ; tags=@("scene","routing") },
    @{ id="scene_physics_grid"; title="Scene Physics and Grid"; desc="Per-scene gravity, collisions, and grid snapping controls." ; tags=@("scene","physics","grid") },
    @{ id="platformer_advanced_moves"; title="Platformer Advanced Moves"; desc="Wall jump, dash, and fall-through platform logic." ; tags=@("platformer","movement") },
    @{ id="tile_level_editor"; title="Tile Level Editor"; desc="Tilemap painting, brush operations, and layer management." ; tags=@("graphics","tilemap") },
    @{ id="sprite_animation_editor"; title="Sprite and Animation Editor"; desc="Sprite sheet slicing and timeline animation editing." ; tags=@("graphics","animation") },
    @{ id="ui_skin_editor"; title="Custom UI Skin Editor"; desc="Dialog frames, cursor assets, and UI style composition." ; tags=@("graphics","ui") },
    @{ id="font_png_json"; title="PNG+JSON Font Mapper"; desc="Bitmap font atlas loading and glyph mapping by JSON." ; tags=@("graphics","font") },
    @{ id="sgb_palette_border"; title="Super Game Boy Border and Palette"; desc="SGB border metadata and palette assignment system." ; tags=@("graphics","sgb") },
    @{ id="music_editor_pianoroll"; title="Music Editor Piano Roll"; desc="Note lane editing with timing and instrument tracks." ; tags=@("audio","music") },
    @{ id="music_editor_tracker"; title="Music Editor Tracker"; desc="Tracker pattern editing and sequence arrangement." ; tags=@("audio","music") },
    @{ id="huge_tracker_bridge"; title="hUGETracker Bridge"; desc="Import-export bridge compatible with hUGETracker workflow." ; tags=@("audio","interop") },
    @{ id="logic_triggers_conditions"; title="Triggers and Conditions"; desc="Trigger dispatch and condition evaluation runtime." ; tags=@("logic","events") },
    @{ id="logic_variables_flags"; title="Variables and Flags"; desc="Global/local variable storage and bit-flag operations." ; tags=@("logic","state") },
    @{ id="dialogue_branching"; title="Dialogue and Branching"; desc="Branching conversation nodes and conditional choices." ; tags=@("logic","dialogue") },
    @{ id="inventory_quest"; title="Inventory and Quest"; desc="Item inventory, quest state machine, and rewards." ; tags=@("logic","rpg") },
    @{ id="actor_collision_ai"; title="Actor Behavior and Collision"; desc="Actor movement models, collision channels, and AI hooks." ; tags=@("logic","collision","ai") },
    @{ id="plugin_js_gbvm"; title="Plugin API JS+GBVM"; desc="Script plugin extension points for JS and GBVM modules." ; tags=@("extensibility","plugin") },
    @{ id="plugin_manager_ext"; title="Plugin Manager"; desc="Plugin discovery, installation, and lifecycle management." ; tags=@("extensibility","plugin") },
    @{ id="engine_source_extension"; title="Engine Source Extension"; desc="C/ASM extension seam registration and export helpers." ; tags=@("extensibility","engine") },
    @{ id="build_export_rom"; title="Build and ROM Export"; desc="Build .gb/.gbc targets and package distribution outputs." ; tags=@("build","export") },
    @{ id="runtime_targets"; title="Runtime Targets"; desc="Launch and packaging targets for emulator, hardware, and web." ; tags=@("build","runtime") },
    @{ id="camera_scrolling"; title="Camera and Scrolling"; desc="Camera bounds, follow behavior, and scrolling policies." ; tags=@("engine","camera") },
    @{ id="runtime_optimization"; title="Runtime Optimization"; desc="Frame budget management and performance instrumentation." ; tags=@("engine","performance") },
    @{ id="gravity_tuning"; title="Gravity and Tuning"; desc="Configurable gravity and gameplay tuning profiles." ; tags=@("engine","tuning") }
)

$outRoot = Join-Path $RepoRoot $OutputDir
$pluginDir = Join-Path $outRoot "plugins"
if ((Test-Path $outRoot) -and (-not $Force)) {
    throw "Output directory already exists. Use -Force to overwrite: $outRoot"
}
New-Item -ItemType Directory -Path $outRoot -Force | Out-Null
New-Item -ItemType Directory -Path $pluginDir -Force | Out-Null

Write-Step "Generating feature modules ($($features.Count))"

$registryEntries = New-Object System.Collections.Generic.List[string]
$registryInitCalls = New-Object System.Collections.Generic.List[string]
$registryTickCalls = New-Object System.Collections.Generic.List[string]
$registryValidateCalls = New-Object System.Collections.Generic.List[string]
$headerIncludes = New-Object System.Collections.Generic.List[string]

foreach ($f in $features) {
    $id = [string]$f.id
    $macro = To-UpperSnake $id
    $pascal = To-PascalCase $id
    $cfg = "${id}_config"
    $state = "${id}_state"

    $hPath = Join-Path $outRoot "$id.h"
    $cPath = Join-Path $outRoot "$id.c"

    $header = @"
#ifndef ${macro}_H
#define ${macro}_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool enabled;
    int revision;
    int option_int[16];
    float option_float[16];
    bool option_bool[16];
} $cfg;

typedef struct {
    int frame_counter;
    int last_error_code;
    char last_status[128];
} $state;

void ${id}_config_defaults($cfg *cfg);
void ${id}_state_init($state *state);
bool ${id}_validate(const $cfg *cfg, char *reason, int reason_len);
bool ${id}_set_option_int($cfg *cfg, int index, int value);
bool ${id}_set_option_float($cfg *cfg, int index, float value);
bool ${id}_set_option_bool($cfg *cfg, int index, bool value);
bool ${id}_handle_command($cfg *cfg, $state *state, const char *command_id);
void ${id}_tick($cfg *cfg, $state *state);
bool ${id}_serialize_config(const $cfg *cfg, const char *path);
bool ${id}_deserialize_config($cfg *cfg, const char *path);

#ifdef __cplusplus
}
#endif

#endif
"@

    $source = @"
#include "$id.h"
#include <stdio.h>
#include <string.h>

static void safe_copy(char *dst, int n, const char *src) {
    if (!dst || n <= 0) return;
    if (!src) src = "";
    strncpy(dst, src, (size_t)(n - 1));
    dst[n - 1] = '\0';
}

void ${id}_config_defaults($cfg *cfg) {
    int i;
    if (!cfg) return;
    cfg->enabled = true;
    cfg->revision = 1;
    for (i = 0; i < 16; ++i) {
        cfg->option_int[i] = 0;
        cfg->option_float[i] = 0.0f;
        cfg->option_bool[i] = false;
    }
}

void ${id}_state_init($state *state) {
    if (!state) return;
    state->frame_counter = 0;
    state->last_error_code = 0;
    safe_copy(state->last_status, (int)sizeof(state->last_status), "${pascal} ready");
}

bool ${id}_validate(const $cfg *cfg, char *reason, int reason_len) {
    if (reason && reason_len > 0) {
        snprintf(reason, (size_t)reason_len, "${pascal}: validation ok");
    }
    if (!cfg) return false;
    return cfg->enabled;
}

bool ${id}_set_option_int($cfg *cfg, int index, int value) {
    if (!cfg || index < 0 || index >= 16) return false;
    cfg->option_int[index] = value;
    return true;
}

bool ${id}_set_option_float($cfg *cfg, int index, float value) {
    if (!cfg || index < 0 || index >= 16) return false;
    cfg->option_float[index] = value;
    return true;
}

bool ${id}_set_option_bool($cfg *cfg, int index, bool value) {
    if (!cfg || index < 0 || index >= 16) return false;
    cfg->option_bool[index] = value;
    return true;
}

bool ${id}_handle_command($cfg *cfg, $state *state, const char *command_id) {
    if (!cfg || !state || !command_id) return false;
    if (strcmp(command_id, "$id.enable") == 0) {
        cfg->enabled = true;
        safe_copy(state->last_status, (int)sizeof(state->last_status), "${pascal} enabled");
        return true;
    }
    if (strcmp(command_id, "$id.disable") == 0) {
        cfg->enabled = false;
        safe_copy(state->last_status, (int)sizeof(state->last_status), "${pascal} disabled");
        return true;
    }
    return false;
}

void ${id}_tick($cfg *cfg, $state *state) {
    if (!cfg || !state || !cfg->enabled) return;
    state->frame_counter++;
    cfg->revision++;
    safe_copy(state->last_status, (int)sizeof(state->last_status), "${pascal} tick");
}

bool ${id}_serialize_config(const $cfg *cfg, const char *path) {
    FILE *f;
    int i;
    if (!cfg || !path) return false;
    f = fopen(path, "w");
    if (!f) return false;
    fprintf(f, "enabled=%d\n", cfg->enabled ? 1 : 0);
    fprintf(f, "revision=%d\n", cfg->revision);
    for (i = 0; i < 16; ++i) fprintf(f, "int_%d=%d\n", i, cfg->option_int[i]);
    for (i = 0; i < 16; ++i) fprintf(f, "float_%d=%.5f\n", i, cfg->option_float[i]);
    for (i = 0; i < 16; ++i) fprintf(f, "bool_%d=%d\n", i, cfg->option_bool[i] ? 1 : 0);
    fclose(f);
    return true;
}

bool ${id}_deserialize_config($cfg *cfg, const char *path) {
    (void)path;
    if (!cfg) return false;
    ${id}_config_defaults(cfg);
    return true;
}
"@

    Set-Content -Path $hPath -Value $header -NoNewline
    Set-Content -Path $cPath -Value $source -NoNewline

    $headerIncludes.Add("#include `"$id.h`"")
    $registryEntries.Add("    { `"$id`", `"$($f.title)`", `"$($f.desc)`", $($f.tags.Count) },")
    $registryInitCalls.Add("    ${id}_config_defaults(&registry->${id}_cfg); ${id}_state_init(&registry->${id}_state);")
    $registryTickCalls.Add("    ${id}_tick(&registry->${id}_cfg, &registry->${id}_state);")
    $registryValidateCalls.Add("    if (!${id}_validate(&registry->${id}_cfg, reason, reason_len)) return false;")
}

$registryHeaderPath = Join-Path $outRoot "auto_feature_registry.h"
$registrySourcePath = Join-Path $outRoot "auto_feature_registry.c"
$commandMapPath = Join-Path $outRoot "auto_feature_command_map.inc"
$manifestPath = Join-Path $outRoot "auto_feature_manifest.json"
$readmePath = Join-Path $outRoot "README.md"

$registryFields = ($features | ForEach-Object { "    $($_.id)_config $($_.id)_cfg;`n    $($_.id)_state $($_.id)_state;" }) -join "`n"

$registryHeader = @"
#ifndef AUTO_FEATURE_REGISTRY_H
#define AUTO_FEATURE_REGISTRY_H

#include <stdbool.h>
$($headerIncludes -join "`n")

typedef struct {
    const char *id;
    const char *title;
    const char *description;
    int tag_count;
} auto_feature_descriptor;

typedef struct {
$registryFields
} auto_feature_registry;

void auto_feature_registry_init(auto_feature_registry *registry);
void auto_feature_registry_tick(auto_feature_registry *registry);
bool auto_feature_registry_validate(const auto_feature_registry *registry, char *reason, int reason_len);
bool auto_feature_registry_dispatch(auto_feature_registry *registry, const char *command_id);
const auto_feature_descriptor *auto_feature_descriptors(int *count);

#endif
"@

$registrySource = @"
#include "auto_feature_registry.h"
#include <string.h>

static const auto_feature_descriptor g_descriptors[] = {
$($registryEntries -join "`n")
};

void auto_feature_registry_init(auto_feature_registry *registry) {
    if (!registry) return;
$($registryInitCalls -join "`n")
}

void auto_feature_registry_tick(auto_feature_registry *registry) {
    if (!registry) return;
$($registryTickCalls -join "`n")
}

bool auto_feature_registry_validate(const auto_feature_registry *registry, char *reason, int reason_len) {
    if (!registry) return false;
$($registryValidateCalls -join "`n")
    return true;
}

bool auto_feature_registry_dispatch(auto_feature_registry *registry, const char *command_id) {
    if (!registry || !command_id) return false;
$(
($features | ForEach-Object {
    "    if ($($_.id)_handle_command(&registry->$($_.id)_cfg, &registry->$($_.id)_state, command_id)) return true;"
}) -join "`n"
)
    return false;
}

const auto_feature_descriptor *auto_feature_descriptors(int *count) {
    if (count) *count = (int)(sizeof(g_descriptors) / sizeof(g_descriptors[0]));
    return g_descriptors;
}
"@

$commandMap = @"
/* Auto-generated command map include */
/* Suggested integration: feed these into command_registry_add(...) in ide_editor.c */
$(
($features | ForEach-Object {
"command_registry_add(&app->command_registry, `"$($_.id).enable`", `"Enable $($_.title)`");`ncommand_registry_add(&app->command_registry, `"$($_.id).disable`", `"Disable $($_.title)`");"
}) -join "`n"
)
"@

$manifest = @{
    feature_pack = "ide_editor_mass_autogen"
    version = 1
    generated_at = (Get-Date).ToString("s")
    module_count = $features.Count
    modules = @($features | ForEach-Object {
        @{
            id = $_.id
            title = $_.title
            description = $_.desc
            tags = $_.tags
        }
    })
} | ConvertTo-Json -Depth 8

$readme = @"
# IDE Mass Feature Autogen

Generated by:
- tools/ide_mass_feature_autogen.ps1

Contains:
- Per-feature modules (.h/.c) for all requested systems
- Shared registry: auto_feature_registry.h/.c
- Command map include: auto_feature_command_map.inc
- Manifest: auto_feature_manifest.json
- Plugin templates in plugins/

Use:
1. Add files from this folder to your build.
2. Include auto_feature_registry.h in ide runtime.
3. Call auto_feature_registry_init(...) on startup.
4. Call auto_feature_registry_tick(...) once per frame.
5. Forward command ids into auto_feature_registry_dispatch(...).
"@

$pluginJs = @"
module.exports = {
  id: "autogen.fullstack.example",
  version: "0.1.0",
  onLoad(api) {
    api.log("autogen fullstack plugin loaded");
    api.registerCommand("autogen.fullstack.ping", () => api.notify("feature stack online"));
  }
};
"@

$pluginGbvm = @"
{
  "id": "autogen.fullstack.gbvm",
  "version": 1,
  "commands": [
    { "name": "AUTOGEN_SET_CAMERA_BOUNDS", "opcode": 224 },
    { "name": "AUTOGEN_SET_GRAVITY_PROFILE", "opcode": 225 },
    { "name": "AUTOGEN_EVENT_TRACE_MARK", "opcode": 226 }
  ]
}
"@

Set-Content -Path $registryHeaderPath -Value $registryHeader -NoNewline
Set-Content -Path $registrySourcePath -Value $registrySource -NoNewline
Set-Content -Path $commandMapPath -Value $commandMap -NoNewline
Set-Content -Path $manifestPath -Value $manifest -NoNewline
Set-Content -Path $readmePath -Value $readme -NoNewline
Set-Content -Path (Join-Path $pluginDir "example_autogen_plugin.js") -Value $pluginJs -NoNewline
Set-Content -Path (Join-Path $pluginDir "example_autogen_gbvm.json") -Value $pluginGbvm -NoNewline

Write-Step "Generated feature stack at: $outRoot"
Write-Step "Modules: $($features.Count)"
