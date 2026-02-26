#include "auto_feature_registry.h"
#include <string.h>

static const auto_feature_descriptor g_descriptors[] = {
    { "visual_game_builder", "Visual Game Builder", "Drag-drop construction tools and scene graph authoring.", 3 },
    { "visual_debugger", "Built-in Debugger", "Breakpoints, watch values, event trace, and step execution.", 2 },
    { "event_logic_rpg", "RPG-style Event Logic", "Event-driven flow similar to RPG Maker command chains.", 2 },
    { "scene_topdown", "Top-down Scene", "Top-down adventure scene runtime and navigation rules.", 2 },
    { "scene_platformer", "Platformer Scene", "Platformer runtime with jump and collision movement rules.", 2 },
    { "scene_shooter", "Shooter Scene", "Shooter runtime with projectile and enemy pattern support.", 2 },
    { "scene_mixer", "Scene Type Mixer", "Combine multiple scene types within a single game project.", 2 },
    { "scene_physics_grid", "Scene Physics and Grid", "Per-scene gravity, collisions, and grid snapping controls.", 3 },
    { "platformer_advanced_moves", "Platformer Advanced Moves", "Wall jump, dash, and fall-through platform logic.", 2 },
    { "tile_level_editor", "Tile Level Editor", "Tilemap painting, brush operations, and layer management.", 2 },
    { "sprite_animation_editor", "Sprite and Animation Editor", "Sprite sheet slicing and timeline animation editing.", 2 },
    { "ui_skin_editor", "Custom UI Skin Editor", "Dialog frames, cursor assets, and UI style composition.", 2 },
    { "font_png_json", "PNG+JSON Font Mapper", "Bitmap font atlas loading and glyph mapping by JSON.", 2 },
    { "sgb_palette_border", "Super Game Boy Border and Palette", "SGB border metadata and palette assignment system.", 2 },
    { "music_editor_pianoroll", "Music Editor Piano Roll", "Note lane editing with timing and instrument tracks.", 2 },
    { "music_editor_tracker", "Music Editor Tracker", "Tracker pattern editing and sequence arrangement.", 2 },
    { "huge_tracker_bridge", "hUGETracker Bridge", "Import-export bridge compatible with hUGETracker workflow.", 2 },
    { "logic_triggers_conditions", "Triggers and Conditions", "Trigger dispatch and condition evaluation runtime.", 2 },
    { "logic_variables_flags", "Variables and Flags", "Global/local variable storage and bit-flag operations.", 2 },
    { "dialogue_branching", "Dialogue and Branching", "Branching conversation nodes and conditional choices.", 2 },
    { "inventory_quest", "Inventory and Quest", "Item inventory, quest state machine, and rewards.", 2 },
    { "actor_collision_ai", "Actor Behavior and Collision", "Actor movement models, collision channels, and AI hooks.", 3 },
    { "plugin_js_gbvm", "Plugin API JS+GBVM", "Script plugin extension points for JS and GBVM modules.", 2 },
    { "plugin_manager_ext", "Plugin Manager", "Plugin discovery, installation, and lifecycle management.", 2 },
    { "engine_source_extension", "Engine Source Extension", "C/ASM extension seam registration and export helpers.", 2 },
    { "build_export_rom", "Build and ROM Export", "Build .gb/.gbc targets and package distribution outputs.", 2 },
    { "runtime_targets", "Runtime Targets", "Launch and packaging targets for emulator, hardware, and web.", 2 },
    { "camera_scrolling", "Camera and Scrolling", "Camera bounds, follow behavior, and scrolling policies.", 2 },
    { "runtime_optimization", "Runtime Optimization", "Frame budget management and performance instrumentation.", 2 },
    { "gravity_tuning", "Gravity and Tuning", "Configurable gravity and gameplay tuning profiles.", 2 },
};

void auto_feature_registry_init(auto_feature_registry *registry) {
    if (!registry) return;
    visual_game_builder_config_defaults(&registry->visual_game_builder_cfg); visual_game_builder_state_init(&registry->visual_game_builder_state);
    visual_debugger_config_defaults(&registry->visual_debugger_cfg); visual_debugger_state_init(&registry->visual_debugger_state);
    event_logic_rpg_config_defaults(&registry->event_logic_rpg_cfg); event_logic_rpg_state_init(&registry->event_logic_rpg_state);
    scene_topdown_config_defaults(&registry->scene_topdown_cfg); scene_topdown_state_init(&registry->scene_topdown_state);
    scene_platformer_config_defaults(&registry->scene_platformer_cfg); scene_platformer_state_init(&registry->scene_platformer_state);
    scene_shooter_config_defaults(&registry->scene_shooter_cfg); scene_shooter_state_init(&registry->scene_shooter_state);
    scene_mixer_config_defaults(&registry->scene_mixer_cfg); scene_mixer_state_init(&registry->scene_mixer_state);
    scene_physics_grid_config_defaults(&registry->scene_physics_grid_cfg); scene_physics_grid_state_init(&registry->scene_physics_grid_state);
    platformer_advanced_moves_config_defaults(&registry->platformer_advanced_moves_cfg); platformer_advanced_moves_state_init(&registry->platformer_advanced_moves_state);
    tile_level_editor_config_defaults(&registry->tile_level_editor_cfg); tile_level_editor_state_init(&registry->tile_level_editor_state);
    sprite_animation_editor_config_defaults(&registry->sprite_animation_editor_cfg); sprite_animation_editor_state_init(&registry->sprite_animation_editor_state);
    ui_skin_editor_config_defaults(&registry->ui_skin_editor_cfg); ui_skin_editor_state_init(&registry->ui_skin_editor_state);
    font_png_json_config_defaults(&registry->font_png_json_cfg); font_png_json_state_init(&registry->font_png_json_state);
    sgb_palette_border_config_defaults(&registry->sgb_palette_border_cfg); sgb_palette_border_state_init(&registry->sgb_palette_border_state);
    music_editor_pianoroll_config_defaults(&registry->music_editor_pianoroll_cfg); music_editor_pianoroll_state_init(&registry->music_editor_pianoroll_state);
    music_editor_tracker_config_defaults(&registry->music_editor_tracker_cfg); music_editor_tracker_state_init(&registry->music_editor_tracker_state);
    huge_tracker_bridge_config_defaults(&registry->huge_tracker_bridge_cfg); huge_tracker_bridge_state_init(&registry->huge_tracker_bridge_state);
    logic_triggers_conditions_config_defaults(&registry->logic_triggers_conditions_cfg); logic_triggers_conditions_state_init(&registry->logic_triggers_conditions_state);
    logic_variables_flags_config_defaults(&registry->logic_variables_flags_cfg); logic_variables_flags_state_init(&registry->logic_variables_flags_state);
    dialogue_branching_config_defaults(&registry->dialogue_branching_cfg); dialogue_branching_state_init(&registry->dialogue_branching_state);
    inventory_quest_config_defaults(&registry->inventory_quest_cfg); inventory_quest_state_init(&registry->inventory_quest_state);
    actor_collision_ai_config_defaults(&registry->actor_collision_ai_cfg); actor_collision_ai_state_init(&registry->actor_collision_ai_state);
    plugin_js_gbvm_config_defaults(&registry->plugin_js_gbvm_cfg); plugin_js_gbvm_state_init(&registry->plugin_js_gbvm_state);
    plugin_manager_ext_config_defaults(&registry->plugin_manager_ext_cfg); plugin_manager_ext_state_init(&registry->plugin_manager_ext_state);
    engine_source_extension_config_defaults(&registry->engine_source_extension_cfg); engine_source_extension_state_init(&registry->engine_source_extension_state);
    build_export_rom_config_defaults(&registry->build_export_rom_cfg); build_export_rom_state_init(&registry->build_export_rom_state);
    runtime_targets_config_defaults(&registry->runtime_targets_cfg); runtime_targets_state_init(&registry->runtime_targets_state);
    camera_scrolling_config_defaults(&registry->camera_scrolling_cfg); camera_scrolling_state_init(&registry->camera_scrolling_state);
    runtime_optimization_config_defaults(&registry->runtime_optimization_cfg); runtime_optimization_state_init(&registry->runtime_optimization_state);
    gravity_tuning_config_defaults(&registry->gravity_tuning_cfg); gravity_tuning_state_init(&registry->gravity_tuning_state);
}

void auto_feature_registry_tick(auto_feature_registry *registry) {
    if (!registry) return;
    visual_game_builder_tick(&registry->visual_game_builder_cfg, &registry->visual_game_builder_state);
    visual_debugger_tick(&registry->visual_debugger_cfg, &registry->visual_debugger_state);
    event_logic_rpg_tick(&registry->event_logic_rpg_cfg, &registry->event_logic_rpg_state);
    scene_topdown_tick(&registry->scene_topdown_cfg, &registry->scene_topdown_state);
    scene_platformer_tick(&registry->scene_platformer_cfg, &registry->scene_platformer_state);
    scene_shooter_tick(&registry->scene_shooter_cfg, &registry->scene_shooter_state);
    scene_mixer_tick(&registry->scene_mixer_cfg, &registry->scene_mixer_state);
    scene_physics_grid_tick(&registry->scene_physics_grid_cfg, &registry->scene_physics_grid_state);
    platformer_advanced_moves_tick(&registry->platformer_advanced_moves_cfg, &registry->platformer_advanced_moves_state);
    tile_level_editor_tick(&registry->tile_level_editor_cfg, &registry->tile_level_editor_state);
    sprite_animation_editor_tick(&registry->sprite_animation_editor_cfg, &registry->sprite_animation_editor_state);
    ui_skin_editor_tick(&registry->ui_skin_editor_cfg, &registry->ui_skin_editor_state);
    font_png_json_tick(&registry->font_png_json_cfg, &registry->font_png_json_state);
    sgb_palette_border_tick(&registry->sgb_palette_border_cfg, &registry->sgb_palette_border_state);
    music_editor_pianoroll_tick(&registry->music_editor_pianoroll_cfg, &registry->music_editor_pianoroll_state);
    music_editor_tracker_tick(&registry->music_editor_tracker_cfg, &registry->music_editor_tracker_state);
    huge_tracker_bridge_tick(&registry->huge_tracker_bridge_cfg, &registry->huge_tracker_bridge_state);
    logic_triggers_conditions_tick(&registry->logic_triggers_conditions_cfg, &registry->logic_triggers_conditions_state);
    logic_variables_flags_tick(&registry->logic_variables_flags_cfg, &registry->logic_variables_flags_state);
    dialogue_branching_tick(&registry->dialogue_branching_cfg, &registry->dialogue_branching_state);
    inventory_quest_tick(&registry->inventory_quest_cfg, &registry->inventory_quest_state);
    actor_collision_ai_tick(&registry->actor_collision_ai_cfg, &registry->actor_collision_ai_state);
    plugin_js_gbvm_tick(&registry->plugin_js_gbvm_cfg, &registry->plugin_js_gbvm_state);
    plugin_manager_ext_tick(&registry->plugin_manager_ext_cfg, &registry->plugin_manager_ext_state);
    engine_source_extension_tick(&registry->engine_source_extension_cfg, &registry->engine_source_extension_state);
    build_export_rom_tick(&registry->build_export_rom_cfg, &registry->build_export_rom_state);
    runtime_targets_tick(&registry->runtime_targets_cfg, &registry->runtime_targets_state);
    camera_scrolling_tick(&registry->camera_scrolling_cfg, &registry->camera_scrolling_state);
    runtime_optimization_tick(&registry->runtime_optimization_cfg, &registry->runtime_optimization_state);
    gravity_tuning_tick(&registry->gravity_tuning_cfg, &registry->gravity_tuning_state);
}

bool auto_feature_registry_validate(const auto_feature_registry *registry, char *reason, int reason_len) {
    if (!registry) return false;
    if (!visual_game_builder_validate(&registry->visual_game_builder_cfg, reason, reason_len)) return false;
    if (!visual_debugger_validate(&registry->visual_debugger_cfg, reason, reason_len)) return false;
    if (!event_logic_rpg_validate(&registry->event_logic_rpg_cfg, reason, reason_len)) return false;
    if (!scene_topdown_validate(&registry->scene_topdown_cfg, reason, reason_len)) return false;
    if (!scene_platformer_validate(&registry->scene_platformer_cfg, reason, reason_len)) return false;
    if (!scene_shooter_validate(&registry->scene_shooter_cfg, reason, reason_len)) return false;
    if (!scene_mixer_validate(&registry->scene_mixer_cfg, reason, reason_len)) return false;
    if (!scene_physics_grid_validate(&registry->scene_physics_grid_cfg, reason, reason_len)) return false;
    if (!platformer_advanced_moves_validate(&registry->platformer_advanced_moves_cfg, reason, reason_len)) return false;
    if (!tile_level_editor_validate(&registry->tile_level_editor_cfg, reason, reason_len)) return false;
    if (!sprite_animation_editor_validate(&registry->sprite_animation_editor_cfg, reason, reason_len)) return false;
    if (!ui_skin_editor_validate(&registry->ui_skin_editor_cfg, reason, reason_len)) return false;
    if (!font_png_json_validate(&registry->font_png_json_cfg, reason, reason_len)) return false;
    if (!sgb_palette_border_validate(&registry->sgb_palette_border_cfg, reason, reason_len)) return false;
    if (!music_editor_pianoroll_validate(&registry->music_editor_pianoroll_cfg, reason, reason_len)) return false;
    if (!music_editor_tracker_validate(&registry->music_editor_tracker_cfg, reason, reason_len)) return false;
    if (!huge_tracker_bridge_validate(&registry->huge_tracker_bridge_cfg, reason, reason_len)) return false;
    if (!logic_triggers_conditions_validate(&registry->logic_triggers_conditions_cfg, reason, reason_len)) return false;
    if (!logic_variables_flags_validate(&registry->logic_variables_flags_cfg, reason, reason_len)) return false;
    if (!dialogue_branching_validate(&registry->dialogue_branching_cfg, reason, reason_len)) return false;
    if (!inventory_quest_validate(&registry->inventory_quest_cfg, reason, reason_len)) return false;
    if (!actor_collision_ai_validate(&registry->actor_collision_ai_cfg, reason, reason_len)) return false;
    if (!plugin_js_gbvm_validate(&registry->plugin_js_gbvm_cfg, reason, reason_len)) return false;
    if (!plugin_manager_ext_validate(&registry->plugin_manager_ext_cfg, reason, reason_len)) return false;
    if (!engine_source_extension_validate(&registry->engine_source_extension_cfg, reason, reason_len)) return false;
    if (!build_export_rom_validate(&registry->build_export_rom_cfg, reason, reason_len)) return false;
    if (!runtime_targets_validate(&registry->runtime_targets_cfg, reason, reason_len)) return false;
    if (!camera_scrolling_validate(&registry->camera_scrolling_cfg, reason, reason_len)) return false;
    if (!runtime_optimization_validate(&registry->runtime_optimization_cfg, reason, reason_len)) return false;
    if (!gravity_tuning_validate(&registry->gravity_tuning_cfg, reason, reason_len)) return false;
    return true;
}

bool auto_feature_registry_dispatch(auto_feature_registry *registry, const char *command_id) {
    if (!registry || !command_id) return false;
    if (visual_game_builder_handle_command(&registry->visual_game_builder_cfg, &registry->visual_game_builder_state, command_id)) return true;
    if (visual_debugger_handle_command(&registry->visual_debugger_cfg, &registry->visual_debugger_state, command_id)) return true;
    if (event_logic_rpg_handle_command(&registry->event_logic_rpg_cfg, &registry->event_logic_rpg_state, command_id)) return true;
    if (scene_topdown_handle_command(&registry->scene_topdown_cfg, &registry->scene_topdown_state, command_id)) return true;
    if (scene_platformer_handle_command(&registry->scene_platformer_cfg, &registry->scene_platformer_state, command_id)) return true;
    if (scene_shooter_handle_command(&registry->scene_shooter_cfg, &registry->scene_shooter_state, command_id)) return true;
    if (scene_mixer_handle_command(&registry->scene_mixer_cfg, &registry->scene_mixer_state, command_id)) return true;
    if (scene_physics_grid_handle_command(&registry->scene_physics_grid_cfg, &registry->scene_physics_grid_state, command_id)) return true;
    if (platformer_advanced_moves_handle_command(&registry->platformer_advanced_moves_cfg, &registry->platformer_advanced_moves_state, command_id)) return true;
    if (tile_level_editor_handle_command(&registry->tile_level_editor_cfg, &registry->tile_level_editor_state, command_id)) return true;
    if (sprite_animation_editor_handle_command(&registry->sprite_animation_editor_cfg, &registry->sprite_animation_editor_state, command_id)) return true;
    if (ui_skin_editor_handle_command(&registry->ui_skin_editor_cfg, &registry->ui_skin_editor_state, command_id)) return true;
    if (font_png_json_handle_command(&registry->font_png_json_cfg, &registry->font_png_json_state, command_id)) return true;
    if (sgb_palette_border_handle_command(&registry->sgb_palette_border_cfg, &registry->sgb_palette_border_state, command_id)) return true;
    if (music_editor_pianoroll_handle_command(&registry->music_editor_pianoroll_cfg, &registry->music_editor_pianoroll_state, command_id)) return true;
    if (music_editor_tracker_handle_command(&registry->music_editor_tracker_cfg, &registry->music_editor_tracker_state, command_id)) return true;
    if (huge_tracker_bridge_handle_command(&registry->huge_tracker_bridge_cfg, &registry->huge_tracker_bridge_state, command_id)) return true;
    if (logic_triggers_conditions_handle_command(&registry->logic_triggers_conditions_cfg, &registry->logic_triggers_conditions_state, command_id)) return true;
    if (logic_variables_flags_handle_command(&registry->logic_variables_flags_cfg, &registry->logic_variables_flags_state, command_id)) return true;
    if (dialogue_branching_handle_command(&registry->dialogue_branching_cfg, &registry->dialogue_branching_state, command_id)) return true;
    if (inventory_quest_handle_command(&registry->inventory_quest_cfg, &registry->inventory_quest_state, command_id)) return true;
    if (actor_collision_ai_handle_command(&registry->actor_collision_ai_cfg, &registry->actor_collision_ai_state, command_id)) return true;
    if (plugin_js_gbvm_handle_command(&registry->plugin_js_gbvm_cfg, &registry->plugin_js_gbvm_state, command_id)) return true;
    if (plugin_manager_ext_handle_command(&registry->plugin_manager_ext_cfg, &registry->plugin_manager_ext_state, command_id)) return true;
    if (engine_source_extension_handle_command(&registry->engine_source_extension_cfg, &registry->engine_source_extension_state, command_id)) return true;
    if (build_export_rom_handle_command(&registry->build_export_rom_cfg, &registry->build_export_rom_state, command_id)) return true;
    if (runtime_targets_handle_command(&registry->runtime_targets_cfg, &registry->runtime_targets_state, command_id)) return true;
    if (camera_scrolling_handle_command(&registry->camera_scrolling_cfg, &registry->camera_scrolling_state, command_id)) return true;
    if (runtime_optimization_handle_command(&registry->runtime_optimization_cfg, &registry->runtime_optimization_state, command_id)) return true;
    if (gravity_tuning_handle_command(&registry->gravity_tuning_cfg, &registry->gravity_tuning_state, command_id)) return true;
    return false;
}

const auto_feature_descriptor *auto_feature_descriptors(int *count) {
    if (count) *count = (int)(sizeof(g_descriptors) / sizeof(g_descriptors[0]));
    return g_descriptors;
}