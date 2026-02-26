#ifndef AUTO_FEATURE_REGISTRY_H
#define AUTO_FEATURE_REGISTRY_H

#include <stdbool.h>
#include "visual_game_builder.h"
#include "visual_debugger.h"
#include "event_logic_rpg.h"
#include "scene_topdown.h"
#include "scene_platformer.h"
#include "scene_shooter.h"
#include "scene_mixer.h"
#include "scene_physics_grid.h"
#include "platformer_advanced_moves.h"
#include "tile_level_editor.h"
#include "sprite_animation_editor.h"
#include "ui_skin_editor.h"
#include "font_png_json.h"
#include "sgb_palette_border.h"
#include "music_editor_pianoroll.h"
#include "music_editor_tracker.h"
#include "huge_tracker_bridge.h"
#include "logic_triggers_conditions.h"
#include "logic_variables_flags.h"
#include "dialogue_branching.h"
#include "inventory_quest.h"
#include "actor_collision_ai.h"
#include "plugin_js_gbvm.h"
#include "plugin_manager_ext.h"
#include "engine_source_extension.h"
#include "build_export_rom.h"
#include "runtime_targets.h"
#include "camera_scrolling.h"
#include "runtime_optimization.h"
#include "gravity_tuning.h"

typedef struct {
    const char *id;
    const char *title;
    const char *description;
    int tag_count;
} auto_feature_descriptor;

typedef struct {
    visual_game_builder_config visual_game_builder_cfg;
    visual_game_builder_state visual_game_builder_state;
    visual_debugger_config visual_debugger_cfg;
    visual_debugger_state visual_debugger_state;
    event_logic_rpg_config event_logic_rpg_cfg;
    event_logic_rpg_state event_logic_rpg_state;
    scene_topdown_config scene_topdown_cfg;
    scene_topdown_state scene_topdown_state;
    scene_platformer_config scene_platformer_cfg;
    scene_platformer_state scene_platformer_state;
    scene_shooter_config scene_shooter_cfg;
    scene_shooter_state scene_shooter_state;
    scene_mixer_config scene_mixer_cfg;
    scene_mixer_state scene_mixer_state;
    scene_physics_grid_config scene_physics_grid_cfg;
    scene_physics_grid_state scene_physics_grid_state;
    platformer_advanced_moves_config platformer_advanced_moves_cfg;
    platformer_advanced_moves_state platformer_advanced_moves_state;
    tile_level_editor_config tile_level_editor_cfg;
    tile_level_editor_state tile_level_editor_state;
    sprite_animation_editor_config sprite_animation_editor_cfg;
    sprite_animation_editor_state sprite_animation_editor_state;
    ui_skin_editor_config ui_skin_editor_cfg;
    ui_skin_editor_state ui_skin_editor_state;
    font_png_json_config font_png_json_cfg;
    font_png_json_state font_png_json_state;
    sgb_palette_border_config sgb_palette_border_cfg;
    sgb_palette_border_state sgb_palette_border_state;
    music_editor_pianoroll_config music_editor_pianoroll_cfg;
    music_editor_pianoroll_state music_editor_pianoroll_state;
    music_editor_tracker_config music_editor_tracker_cfg;
    music_editor_tracker_state music_editor_tracker_state;
    huge_tracker_bridge_config huge_tracker_bridge_cfg;
    huge_tracker_bridge_state huge_tracker_bridge_state;
    logic_triggers_conditions_config logic_triggers_conditions_cfg;
    logic_triggers_conditions_state logic_triggers_conditions_state;
    logic_variables_flags_config logic_variables_flags_cfg;
    logic_variables_flags_state logic_variables_flags_state;
    dialogue_branching_config dialogue_branching_cfg;
    dialogue_branching_state dialogue_branching_state;
    inventory_quest_config inventory_quest_cfg;
    inventory_quest_state inventory_quest_state;
    actor_collision_ai_config actor_collision_ai_cfg;
    actor_collision_ai_state actor_collision_ai_state;
    plugin_js_gbvm_config plugin_js_gbvm_cfg;
    plugin_js_gbvm_state plugin_js_gbvm_state;
    plugin_manager_ext_config plugin_manager_ext_cfg;
    plugin_manager_ext_state plugin_manager_ext_state;
    engine_source_extension_config engine_source_extension_cfg;
    engine_source_extension_state engine_source_extension_state;
    build_export_rom_config build_export_rom_cfg;
    build_export_rom_state build_export_rom_state;
    runtime_targets_config runtime_targets_cfg;
    runtime_targets_state runtime_targets_state;
    camera_scrolling_config camera_scrolling_cfg;
    camera_scrolling_state camera_scrolling_state;
    runtime_optimization_config runtime_optimization_cfg;
    runtime_optimization_state runtime_optimization_state;
    gravity_tuning_config gravity_tuning_cfg;
    gravity_tuning_state gravity_tuning_state;
} auto_feature_registry;

void auto_feature_registry_init(auto_feature_registry *registry);
void auto_feature_registry_tick(auto_feature_registry *registry);
bool auto_feature_registry_validate(const auto_feature_registry *registry, char *reason, int reason_len);
bool auto_feature_registry_dispatch(auto_feature_registry *registry, const char *command_id);
const auto_feature_descriptor *auto_feature_descriptors(int *count);

#endif