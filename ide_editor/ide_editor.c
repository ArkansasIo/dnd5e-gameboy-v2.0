#include "ide_editor.h"
#include "menu.h"
#include "sidebar.h"
#include "widget.h"
#include "asset_tools.h"
#include "game_engine_tools.h"
#include "gb_studio_tools.h"
#include "tool_modes.h"
#include "project_io.h"
#include "build_runner.h"
#include "settings_ui.h"
#include "help_about.h"
#include "source_tools.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <conio.h>
#include <windows.h>

#define KEY_EXT_BASE 1000
#define KEY_UP (KEY_EXT_BASE + 72)
#define KEY_DOWN (KEY_EXT_BASE + 80)
#define KEY_LEFT (KEY_EXT_BASE + 75)
#define KEY_RIGHT (KEY_EXT_BASE + 77)
#define KEY_F1 (KEY_EXT_BASE + 59)
#define KEY_F2 (KEY_EXT_BASE + 60)
#define KEY_F5 (KEY_EXT_BASE + 63)
#define KEY_F6 (KEY_EXT_BASE + 64)
#define KEY_F7 (KEY_EXT_BASE + 65)
#define KEY_F8 (KEY_EXT_BASE + 66)
#define KEY_F10 (KEY_EXT_BASE + 68)

static void register_commands(IDEEditorApp *app) {
    command_registry_add(&app->command_registry, "file.new_project", "Create a new project workspace");
    command_registry_add(&app->command_registry, "file.open_project", "Open existing project file");
    command_registry_add(&app->command_registry, "file.save_project", "Save project file");
    command_registry_add(&app->command_registry, "file.save_all", "Save all open assets and maps");
    command_registry_add(&app->command_registry, "file.import_asset", "Import external asset");
    command_registry_add(&app->command_registry, "file.export_manifest", "Generate asset manifest");

    command_registry_add(&app->command_registry, "edit.undo", "Undo last edit operation");
    command_registry_add(&app->command_registry, "edit.redo", "Redo last undone operation");

    command_registry_add(&app->command_registry, "tools.paint", "Activate paint tool");
    command_registry_add(&app->command_registry, "tools.fill", "Activate fill tool");
    command_registry_add(&app->command_registry, "tools.erase", "Activate erase tool");
    command_registry_add(&app->command_registry, "tools.object", "Activate object placement tool");

    command_registry_add(&app->command_registry, "view.theme.classic", "Switch to Classic Dark theme");
    command_registry_add(&app->command_registry, "view.theme.blueprint", "Switch to Blueprint Noir theme");
    command_registry_add(&app->command_registry, "view.theme.retro", "Switch to Retro Amber theme");
    command_registry_add(&app->command_registry, "view.theme.contrast", "Switch to High Contrast theme");

    command_registry_add(&app->command_registry, "engine.validate.encounter", "Validate encounter budget");
    command_registry_add(&app->command_registry, "engine.rebuild.worldgen", "Trigger worldgen rebuild hook");

    command_registry_add(&app->command_registry, "gb.scene.new", "Create GB Studio scene");
    command_registry_add(&app->command_registry, "gb.actor.add", "Add GB Studio actor to active scene");
    command_registry_add(&app->command_registry, "gb.trigger.add", "Add trigger in active scene");
    command_registry_add(&app->command_registry, "gb.script.add", "Create sample script event");
    command_registry_add(&app->command_registry, "gb.script.run_last", "Run latest script event");
    command_registry_add(&app->command_registry, "gb.validate.scene", "Validate active scene limits");
    command_registry_add(&app->command_registry, "gb.validate.all", "Validate all GB Studio scenes");
    command_registry_add(&app->command_registry, "gb.scene.next", "Switch to next scene");
    command_registry_add(&app->command_registry, "gb.scene.prev", "Switch to previous scene");
    command_registry_add(&app->command_registry, "gb.actor.add_hostile", "Add hostile actor to active scene");
    command_registry_add(&app->command_registry, "gb.script.add.reward", "Create reward script (+gold)");
    command_registry_add(&app->command_registry, "gb.script.add.damage", "Create damage script");
    command_registry_add(&app->command_registry, "gb.play.tick", "Advance GB gameplay logic by one tick");
    command_registry_add(&app->command_registry, "gb.play.toggle", "Toggle GB play mode");
    command_registry_add(&app->command_registry, "gb.project.save", "Save GB Studio project data");
    command_registry_add(&app->command_registry, "gb.project.load", "Load GB Studio project data");

    command_registry_add(&app->command_registry, "source.scan", "Scan source folder");
    command_registry_add(&app->command_registry, "source.next", "Next source file");
    command_registry_add(&app->command_registry, "source.prev", "Previous source file");
    command_registry_add(&app->command_registry, "source.open", "Open active source file");
    command_registry_add(&app->command_registry, "source.save", "Save active source file");
    command_registry_add(&app->command_registry, "source.compile.active", "Compile active source file");
    command_registry_add(&app->command_registry, "source.compile.asm", "Compile active source as assembly");
    command_registry_add(&app->command_registry, "source.compile.c", "Compile active source as C");
}

static void add_menu(IDEEditorApp *app, const char *path, const char *command_id) {
    if (!app || !path || !command_id) return;
    menu_add_item(app->top_menu, path, command_id);
    ui_api_add_menu_item(&app->ui, path, command_id);
}

static void build_menus(IDEEditorApp *app) {
    /* Unreal-style menu bar with GB Studio workflows */
    add_menu(app, "File/New Project", "file.new_project");
    add_menu(app, "File/Open Project", "file.open_project");
    add_menu(app, "File/Save Project", "file.save_project");
    add_menu(app, "File/Save All", "file.save_all");
    add_menu(app, "File/Import/Asset", "file.import_asset");
    add_menu(app, "File/Import/Map Data", "file.open_project");
    add_menu(app, "File/Source/Scan src", "source.scan");
    add_menu(app, "File/Source/Next File", "source.next");
    add_menu(app, "File/Source/Previous File", "source.prev");
    add_menu(app, "File/Source/Open Active", "source.open");
    add_menu(app, "File/Source/Save Active", "source.save");
    add_menu(app, "File/Export/Manifest", "file.export_manifest");
    add_menu(app, "File/Export/Build Package", "file.export_manifest");

    add_menu(app, "Edit/Undo", "edit.undo");
    add_menu(app, "Edit/Redo", "edit.redo");
    add_menu(app, "Edit/Duplicate Selection", "edit.redo");
    add_menu(app, "Edit/Delete Selection", "edit.undo");
    add_menu(app, "Edit/Editor Preferences", "file.open_project");

    add_menu(app, "View/Themes/Classic Dark", "view.theme.classic");
    add_menu(app, "View/Themes/Blueprint Noir", "view.theme.blueprint");
    add_menu(app, "View/Themes/Retro Amber", "view.theme.retro");
    add_menu(app, "View/Themes/High Contrast", "view.theme.contrast");
    add_menu(app, "View/Panels/World Hierarchy", "file.open_project");
    add_menu(app, "View/Panels/Asset Outliner", "file.open_project");
    add_menu(app, "View/Panels/Details", "file.open_project");
    add_menu(app, "View/Panels/Output Log", "file.open_project");
    add_menu(app, "View/Panels/Profiler", "file.open_project");

    add_menu(app, "Tools/Terrain/Paint", "tools.paint");
    add_menu(app, "Tools/Terrain/Fill", "tools.fill");
    add_menu(app, "Tools/Terrain/Erase", "tools.erase");
    add_menu(app, "Tools/Terrain/Collision Brush", "tools.erase");
    add_menu(app, "Tools/Objects/Place", "tools.object");
    add_menu(app, "Tools/Objects/Move", "tools.object");
    add_menu(app, "Tools/Objects/Rotate", "tools.object");
    add_menu(app, "Tools/Objects/Scale", "tools.object");
    add_menu(app, "Tools/Debug/Validate Encounter", "engine.validate.encounter");
    add_menu(app, "Tools/Debug/Rebuild Worldgen", "engine.rebuild.worldgen");

    add_menu(app, "Blueprint/New Graph", "file.new_project");
    add_menu(app, "Blueprint/Compile Graph", "engine.rebuild.worldgen");
    add_menu(app, "Blueprint/Validate Graph", "engine.validate.encounter");

    add_menu(app, "Build/Build Project", "engine.rebuild.worldgen");
    add_menu(app, "Build/Run Game", "gb.play.toggle");
    add_menu(app, "Build/Validate Data", "gb.validate.all");
    add_menu(app, "Build/Compile/Active Source", "source.compile.active");
    add_menu(app, "Build/Compile/Active C", "source.compile.c");
    add_menu(app, "Build/Compile/Active ASM", "source.compile.asm");

    add_menu(app, "GB Studio/Scenes/New Scene", "gb.scene.new");
    add_menu(app, "GB Studio/Scenes/Next Scene", "gb.scene.next");
    add_menu(app, "GB Studio/Scenes/Previous Scene", "gb.scene.prev");
    add_menu(app, "GB Studio/Scenes/Validate Active Scene", "gb.validate.scene");
    add_menu(app, "GB Studio/Actors/Add NPC", "gb.actor.add");
    add_menu(app, "GB Studio/Actors/Add Hostile", "gb.actor.add_hostile");
    add_menu(app, "GB Studio/Actors/Auto Place Patrol", "gb.actor.add_hostile");
    add_menu(app, "GB Studio/Triggers/Add Trigger", "gb.trigger.add");
    add_menu(app, "GB Studio/Triggers/Run Last Trigger Script", "gb.script.run_last");
    add_menu(app, "GB Studio/Scripts/New Script Event", "gb.script.add");
    add_menu(app, "GB Studio/Scripts/New Reward Script", "gb.script.add.reward");
    add_menu(app, "GB Studio/Scripts/New Damage Script", "gb.script.add.damage");
    add_menu(app, "GB Studio/Scripts/Run Last Script Event", "gb.script.run_last");
    add_menu(app, "GB Studio/Play/Single Tick", "gb.play.tick");
    add_menu(app, "GB Studio/Play/Toggle Play Mode", "gb.play.toggle");
    add_menu(app, "GB Studio/Build/Validate Active Scene", "gb.validate.scene");
    add_menu(app, "GB Studio/Build/Validate All Scenes", "gb.validate.all");
    add_menu(app, "GB Studio/Project/Save", "gb.project.save");
    add_menu(app, "GB Studio/Project/Load", "gb.project.load");
    add_menu(app, "GB Studio/Project/Export ROM Config", "gb.project.save");

    add_menu(app, "Window/Layout/Load Unreal GB Preset", "file.open_project");
    add_menu(app, "Window/Layout/Save Current Layout", "file.save_project");
    add_menu(app, "Window/Layout/Reset Layout", "file.open_project");

    add_menu(app, "Help/Quick Start", "file.open_project");
    add_menu(app, "Help/Controls", "file.open_project");
    add_menu(app, "Help/About", "file.open_project");
}

static void build_ui_layout(IDEEditorApp *app) {
    /* Primary dock layout */
    ui_api_add_panel(&app->ui, "panel.hierarchy", "World Hierarchy", UI_DOCK_LEFT);
    ui_api_add_panel(&app->ui, "panel.outliner", "Asset Outliner", UI_DOCK_LEFT);
    ui_api_add_panel(&app->ui, "panel.scene_tree", "Scene Tree", UI_DOCK_LEFT);
    ui_api_add_panel(&app->ui, "panel.actor_list", "Actor List", UI_DOCK_LEFT);
    ui_api_add_panel(&app->ui, "panel.viewport", "2D Viewport", UI_DOCK_CENTER);
    ui_api_add_panel(&app->ui, "panel.game_preview", "Game Preview", UI_DOCK_CENTER);
    ui_api_add_panel(&app->ui, "panel.minimap", "Mini Map", UI_DOCK_CENTER);
    ui_api_add_panel(&app->ui, "panel.details", "Details", UI_DOCK_RIGHT);
    ui_api_add_panel(&app->ui, "panel.inspector", "Inspector", UI_DOCK_RIGHT);
    ui_api_add_panel(&app->ui, "panel.palette", "Tile Palette", UI_DOCK_RIGHT);
    ui_api_add_panel(&app->ui, "panel.content", "Content Browser", UI_DOCK_BOTTOM);
    ui_api_add_panel(&app->ui, "panel.console", "Output Log", UI_DOCK_BOTTOM);
    ui_api_add_panel(&app->ui, "panel.timeline", "Timeline", UI_DOCK_BOTTOM);
    ui_api_add_panel(&app->ui, "panel.command_palette", "Command Palette", UI_DOCK_BOTTOM);
    ui_api_add_panel(&app->ui, "panel.blueprint", "Blueprint Graph", UI_DOCK_CENTER);
    ui_api_add_panel(&app->ui, "panel.profiler", "Perf/Memory", UI_DOCK_RIGHT);

    /* Toolbar buttons */
    ui_api_add_widget(&app->ui, "widget.toolbar.new", "panel.viewport", "New", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.toolbar.open", "panel.viewport", "Open", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.toolbar.save", "panel.viewport", "Save", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.toolbar.play", "panel.viewport", "Play", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.toolbar.stop", "panel.viewport", "Stop", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.toolbar.build", "panel.viewport", "Build", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.toolbar.validate", "panel.viewport", "Validate", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.toolbar.scan_src", "panel.viewport", "Scan Src", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.toolbar.compile_src", "panel.viewport", "Compile Src", UI_WIDGET_BUTTON);

    /* Tool shelf buttons */
    ui_api_add_widget(&app->ui, "widget.tools.select", "panel.palette", "Select", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.tools.move", "panel.palette", "Move", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.tools.rotate", "panel.palette", "Rotate", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.tools.scale", "panel.palette", "Scale", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.tools.paint", "panel.palette", "Paint", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.tools.fill", "panel.palette", "Fill", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.tools.erase", "panel.palette", "Erase", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.tools.actor", "panel.palette", "Actor", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.tools.trigger", "panel.palette", "Trigger", UI_WIDGET_BUTTON);
    ui_api_add_widget(&app->ui, "widget.tools.script", "panel.palette", "Script", UI_WIDGET_BUTTON);

    /* Options / details controls */
    ui_api_add_widget(&app->ui, "widget.viewport.grid", "panel.viewport", "Show Grid", UI_WIDGET_TOGGLE);
    ui_api_add_widget(&app->ui, "widget.viewport.snap", "panel.viewport", "Snap To Tile", UI_WIDGET_TOGGLE);
    ui_api_add_widget(&app->ui, "widget.viewport.collisions", "panel.viewport", "Show Collisions", UI_WIDGET_TOGGLE);
    ui_api_add_widget(&app->ui, "widget.details.layer", "panel.details", "Layer", UI_WIDGET_INPUT);
    ui_api_add_widget(&app->ui, "widget.details.name", "panel.details", "Selected Name", UI_WIDGET_INPUT);
    ui_api_add_widget(&app->ui, "widget.details.tags", "panel.details", "Tags", UI_WIDGET_INPUT);
    ui_api_add_widget(&app->ui, "widget.details.speed", "panel.details", "Move Speed", UI_WIDGET_SLIDER);
    ui_api_add_widget(&app->ui, "widget.details.hp", "panel.details", "Actor HP", UI_WIDGET_SLIDER);
    ui_api_add_widget(&app->ui, "widget.console.filter", "panel.console", "Filter", UI_WIDGET_INPUT);
    ui_api_add_widget(&app->ui, "widget.console.search", "panel.console", "Search", UI_WIDGET_INPUT);
    ui_api_add_widget(&app->ui, "widget.palette.brush", "panel.palette", "Brush", UI_WIDGET_INPUT);
    ui_api_add_widget(&app->ui, "widget.palette.size", "panel.palette", "Brush Size", UI_WIDGET_SLIDER);

    /* Initial values */
    ui_api_set_widget_text(&app->ui, "widget.details.name", "None");
    ui_api_set_widget_text(&app->ui, "widget.details.layer", "Base");
    ui_api_set_widget_text(&app->ui, "widget.details.tags", "interactive");
    ui_api_set_widget_text(&app->ui, "widget.console.filter", "all");
    ui_api_set_widget_text(&app->ui, "widget.console.search", "");
    ui_api_set_widget_text(&app->ui, "widget.palette.brush", "Solid");
}

static void build_sidebar(IDEEditorApp *app) {
    sidebar_add_item(app->sidebar, "Project Explorer");
    sidebar_add_item(app->sidebar, "World Database");
    sidebar_add_item(app->sidebar, "Scene Outliner");
    sidebar_add_item(app->sidebar, "Actor Browser");
    sidebar_add_item(app->sidebar, "Event Graph");
    sidebar_add_item(app->sidebar, "Tilemap Layers");
    sidebar_add_item(app->sidebar, "Object Palette");
    sidebar_add_item(app->sidebar, "Brush and Stamps");
    sidebar_add_item(app->sidebar, "GB Script Library");
    sidebar_add_item(app->sidebar, "Blueprint Tools");
    sidebar_add_item(app->sidebar, "Build and Deploy");
    sidebar_add_item(app->sidebar, "Debug and Validation");
    sidebar_add_item(app->sidebar, "Layout Presets");
    sidebar_add_item(app->sidebar, "GB Studio Workflow");
}

static void register_tools(IDEEditorApp *app) {
    editor_tools_register(&app->tool_registry, "Select", "Common", "V", true);
    editor_tools_register(&app->tool_registry, "Move", "Common", "W", true);
    editor_tools_register(&app->tool_registry, "Rotate", "Common", "E", true);
    editor_tools_register(&app->tool_registry, "Scale", "Common", "R", true);

    editor_tools_register(&app->tool_registry, "Paint Tile", "Terrain", "1", true);
    editor_tools_register(&app->tool_registry, "Fill Region", "Terrain", "2", true);
    editor_tools_register(&app->tool_registry, "Erase Tile", "Terrain", "3", true);
    editor_tools_register(&app->tool_registry, "Collision Brush", "Terrain", "4", true);

    editor_tools_register(&app->tool_registry, "Place Spawn", "Gameplay", "5", true);
    editor_tools_register(&app->tool_registry, "Zone Trigger", "Gameplay", "6", true);
    editor_tools_register(&app->tool_registry, "NPC Path", "Gameplay", "7", true);
    editor_tools_register(&app->tool_registry, "Loot Marker", "Gameplay", "8", true);

    editor_tools_register(&app->tool_registry, "Scene Paint", "GB Studio", "G", true);
    editor_tools_register(&app->tool_registry, "Actor Place", "GB Studio", "H", true);
    editor_tools_register(&app->tool_registry, "Trigger Draw", "GB Studio", "J", true);
    editor_tools_register(&app->tool_registry, "Script Event", "GB Studio", "K", true);
    editor_tools_register(&app->tool_registry, "Scene Navigator", "GB Studio", "N", true);
    editor_tools_register(&app->tool_registry, "Dialogue Node", "GB Studio", "D", true);
    editor_tools_register(&app->tool_registry, "Warp Door", "GB Studio", "W", true);
    editor_tools_register(&app->tool_registry, "Encounter Zone", "GB Studio", "Z", true);

    editor_tools_set_active(&app->tool_registry, 5);
}

static void editor_seed_default_content(IDEEditorApp *app) {
    int map_id;
    if (!app) return;

    map_id = project_db_add_asset(app->project_db, DB_ASSET_TILEMAP, "start_zone", "res/maps/start_zone.csv", "zone-map");
    if (map_id > 0) {
        project_db_add_asset(app->project_db, DB_ASSET_TILESET, "dungeon_tiles", "res/tiles/dungeon.bin", "tileset");
        project_db_add_asset(app->project_db, DB_ASSET_SCRIPT, "act01_intro", "worldgen/generated/story_mode_og_12acts_50chapters.json", "story");
        project_db_add_asset(app->project_db, DB_ASSET_DATA, "encounters", "worldgen/generated/encounter_tables.json", "balance");
        project_db_add_asset(app->project_db, DB_ASSET_DATA, "classes", "worldgen/generated/player_classes_5e.json", "rpg");
    }
}

static void gbstudio_seed_default_content(IDEEditorApp *app) {
    int scene_id;
    int event_id;
    if (!app) return;

    scene_id = gbstudio_add_scene(&app->gb_project, "Forest Entry", 32, 24, 1);
    if (scene_id <= 0) return;

    gbstudio_add_actor(&app->gb_project, scene_id, "Guide NPC", "npc_guide", 4, 6, 1, false);
    gbstudio_add_actor(&app->gb_project, scene_id, "Wolf", "enemy_wolf", 18, 9, 1, true);
    event_id = gbstudio_add_script_event(&app->gb_project, GB_SCRIPT_ADD_GOLD, 10, 0, 0);
    gbstudio_add_trigger(&app->gb_project, scene_id, "Chest Trigger", 10, 10, 2, 2, event_id);
}

static void editor_demo_apply_tools(IDEEditorApp *app) {
    ToolPoint p;
    EncounterBudget b;
    char reason[128];
    if (!app || !app->active_map) return;

    p.x = 3; p.y = 3;
    tool_apply(TOOL_PAINT, app->active_map, &app->commands, p, app->selected_tile_id);
    p.x = 10; p.y = 10;
    tool_apply(TOOL_FILL, app->active_map, &app->commands, p, 8);
    p.x = 2; p.y = 2;
    tool_apply(TOOL_ERASE, app->active_map, &app->commands, p, 0);

    tool_undo_last(app->active_map, &app->commands);
    tool_redo_last(app->active_map, &app->commands);

    b.chapter = 1;
    b.recommended_party_level_min = 1;
    b.recommended_party_level_max = 3;
    b.enemy_cr_total = 6;
    game_tools_validate_encounter(&b, reason, sizeof(reason));
}

static void apply_theme(IDEEditorApp *app, EditorThemeId id) {
    char text[96];
    app->theme_id = id;
    ui_api_apply_theme(&app->ui, id);
    snprintf(text, sizeof(text), "Theme switched: %s", editor_theme_name(id));
    ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, text, 180);
    printf("[IDE] Theme switched: %s\n", editor_theme_name(id));
}

static int appendf(char *buf, int cap, int pos, const char *fmt, ...) {
    va_list args;
    int wrote;
    if (!buf || cap <= 0 || pos >= cap) return pos;
    va_start(args, fmt);
    wrote = _vsnprintf(buf + pos, (size_t)(cap - pos), fmt, args);
    va_end(args);
    if (wrote < 0) return cap - 1;
    pos += wrote;
    if (pos >= cap) pos = cap - 1;
    buf[pos] = '\0';
    return pos;
}

static void execute_command(IDEEditorApp *app, const char *command_id);

static void build_overlay_text(IDEEditorApp *app, char *out, int out_len) {
    int i;
    int pos = 0;
    const int max_menu_show = 28;
    const int max_tool_show = 18;
    const int max_widget_show = 18;
    if (!app || !out || out_len <= 0) return;
    out[0] = '\0';

    pos = appendf(out, out_len, pos, "Labyrinth IDE - GUI API\n");
    pos = appendf(out, out_len, pos, "Theme: %s  Backend: %s\n", editor_theme_name(app->theme_id), editor_backend_name(app->backend));
    pos = appendf(out, out_len, pos, "Play:%s  Scene:%d  HP:%d/%d  Gold:%d\n\n",
        app->gb_play_mode ? "ON" : "OFF",
        app->gb_project.active_scene_id,
        app->gb_project.player_hp,
        app->gb_project.player_hp_max,
        app->gb_project.player_gold);

    pos = appendf(out, out_len, pos, "Menus/Submenus:\n");
    for (i = 0; i < menu_item_count(app->top_menu) && i < max_menu_show; i++) {
        const char *path = NULL;
        const char *cmd = NULL;
        if (menu_get_item(app->top_menu, i, &path, &cmd)) {
            pos = appendf(out, out_len, pos, "  - %s -> %s\n", path, cmd);
        }
    }

    pos = appendf(out, out_len, pos, "\nTools:\n");
    for (i = 0; i < app->tool_registry.count && i < max_tool_show; i++) {
        const EditorToolDef *t = &app->tool_registry.tools[i];
        pos = appendf(out, out_len, pos, "  %c %s [%s] (%s)\n",
            (t->id == app->tool_registry.active_tool_id) ? '*' : ' ',
            t->name, t->category, t->shortcut);
    }

    pos = appendf(out, out_len, pos, "\nWidgets/Buttons:\n");
    for (i = 0; i < app->ui.widget_count && i < max_widget_show; i++) {
        const UIWidget *w = &app->ui.widgets[i];
        pos = appendf(out, out_len, pos, "  - %s [%s]\n", w->label, w->panel_id);
    }

    {
        const SourceFileEntry *active = source_workspace_active(&app->sources);
        pos = appendf(out, out_len, pos, "\nSource Workspace:\n");
        pos = appendf(out, out_len, pos, "  Root: %s\n", app->sources.root);
        pos = appendf(out, out_len, pos, "  Files: %d  Active: %d\n",
            app->sources.file_count,
            app->sources.active_index + 1);
        pos = appendf(out, out_len, pos, "  Active File: %s\n",
            active ? active->name : "(none)");
        pos = appendf(out, out_len, pos, "  Status: %s\n", app->sources.last_status);
    }

    pos = appendf(out, out_len, pos, "\nHotkeys: TAB interactive menu, G/H/J/K/V/B/N/X/L/Y/Z + Q/Esc quit");
    menu_interaction_append_overlay(&app->interactive_menu, out + pos, out_len - pos);
}

static void ui_action_add(EditorUIAction *actions, int *count, int max_count, int x, int y, int w, int h, const char *label, const char *command_id) {
    EditorUIAction *a;
    if (!actions || !count || *count < 0 || *count >= max_count) return;
    a = &actions[*count];
    a->x = x;
    a->y = y;
    a->w = w;
    a->h = h;
    strncpy(a->label, label ? label : "", sizeof(a->label) - 1);
    a->label[sizeof(a->label) - 1] = '\0';
    strncpy(a->command_id, command_id ? command_id : "", sizeof(a->command_id) - 1);
    a->command_id[sizeof(a->command_id) - 1] = '\0';
    (*count)++;
}

static void build_ui_actions(IDEEditorApp *app, EditorUIAction *actions, int *out_count, int max_count) {
    int count = 0;
    int i;
    int x = 10;
    const int top_h = 30;
    const int menu_h = 24;
    const int tool_h = 34;
    const int menu_y = top_h;
    const int toolbar_y = top_h + menu_h;
    const int right_x = 1010;
    if (!app || !actions || !out_count) return;

    for (i = 0; i < app->interactive_menu.top_count && i < 9; i++) {
        char cmd[32];
        snprintf(cmd, sizeof(cmd), "ui.menu.top.%d", i);
        ui_action_add(actions, &count, max_count, x, menu_y + 2, 92, menu_h - 4, app->interactive_menu.tops[i].name, cmd);
        x += 96;
    }

    if (app->mouse_menu_open && app->mouse_menu_top >= 0 && app->mouse_menu_top < app->interactive_menu.top_count) {
        const InteractiveMenuTop *top = &app->interactive_menu.tops[app->mouse_menu_top];
        int item_y = menu_y + menu_h;
        int menu_x = 10 + app->mouse_menu_top * 96;
        for (i = 0; i < top->leaf_count && i < 14; i++) {
            ui_action_add(actions, &count, max_count, menu_x, item_y, 240, 22, top->leaves[i].label, top->leaves[i].command_id);
            item_y += 22;
        }
    }

    ui_action_add(actions, &count, max_count, 10, toolbar_y + 4, 64, tool_h - 8, "New", "file.new_project");
    ui_action_add(actions, &count, max_count, 78, toolbar_y + 4, 64, tool_h - 8, "Open", "file.open_project");
    ui_action_add(actions, &count, max_count, 146, toolbar_y + 4, 64, tool_h - 8, "Save", "file.save_project");
    ui_action_add(actions, &count, max_count, 214, toolbar_y + 4, 64, tool_h - 8, "Undo", "edit.undo");
    ui_action_add(actions, &count, max_count, 282, toolbar_y + 4, 64, tool_h - 8, "Redo", "edit.redo");
    ui_action_add(actions, &count, max_count, 350, toolbar_y + 4, 64, tool_h - 8, "Build", "engine.rebuild.worldgen");
    ui_action_add(actions, &count, max_count, 418, toolbar_y + 4, 64, tool_h - 8, "Play", "gb.play.toggle");
    ui_action_add(actions, &count, max_count, 486, toolbar_y + 4, 88, tool_h - 8, "Validate", "gb.validate.all");
    ui_action_add(actions, &count, max_count, 578, toolbar_y + 4, 84, tool_h - 8, "Scan Src", "source.scan");
    ui_action_add(actions, &count, max_count, 666, toolbar_y + 4, 104, tool_h - 8, "Compile Src", "source.compile.active");

    ui_action_add(actions, &count, max_count, right_x, 140, 120, 24, "Paint", "tools.paint");
    ui_action_add(actions, &count, max_count, right_x, 168, 120, 24, "Fill", "tools.fill");
    ui_action_add(actions, &count, max_count, right_x, 196, 120, 24, "Erase", "tools.erase");
    ui_action_add(actions, &count, max_count, right_x, 224, 120, 24, "Obj", "tools.object");
    ui_action_add(actions, &count, max_count, right_x, 270, 120, 24, "Scene+", "gb.scene.new");
    ui_action_add(actions, &count, max_count, right_x, 298, 120, 24, "Actor+", "gb.actor.add");
    ui_action_add(actions, &count, max_count, right_x, 326, 120, 24, "Trigger+", "gb.trigger.add");
    ui_action_add(actions, &count, max_count, right_x, 354, 120, 24, "ScriptRun", "gb.script.run_last");

    *out_count = count;
}

static void handle_ui_action(IDEEditorApp *app, const char *command_id) {
    if (!app || !command_id) return;
    if (strncmp(command_id, "ui.menu.top.", 12) == 0) {
        int idx = atoi(command_id + 12);
        if (idx >= 0 && idx < app->interactive_menu.top_count) {
            if (app->mouse_menu_open && app->mouse_menu_top == idx) {
                app->mouse_menu_open = false;
                app->mouse_menu_top = -1;
            } else {
                app->mouse_menu_open = true;
                app->mouse_menu_top = idx;
            }
        }
        return;
    }
    execute_command(app, command_id);
    app->mouse_menu_open = false;
    app->mouse_menu_top = -1;
}

static void execute_command(IDEEditorApp *app, const char *command_id) {
    if (!app || !command_id) return;

    if (strcmp(command_id, "edit.undo") == 0) {
        tool_undo_last(app->active_map, &app->commands);
    } else if (strcmp(command_id, "edit.redo") == 0) {
        tool_redo_last(app->active_map, &app->commands);
    } else if (strcmp(command_id, "file.save_project") == 0) {
        project_io_save("autosave_project.db", app->project_db, app->active_map);
        editor_settings_save(&app->settings, "editor_settings.ini");
        ui_api_save_layout(&app->ui, "editor_ui_layout.ini");
        ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, "Project/settings/UI layout saved", 180);
        printf("[IDE] Project and settings saved.\n");
    } else if (strcmp(command_id, "file.open_project") == 0) {
        project_io_load("autosave_project.db", app->project_db);
        editor_settings_load(&app->settings, "editor_settings.ini");
        ui_api_load_layout(&app->ui, "editor_ui_layout.ini");
        ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, "Project/settings/UI layout loaded", 180);
        printf("[IDE] Project and settings loaded.\n");
    } else if (strcmp(command_id, "file.export_manifest") == 0) {
        asset_tools_generate_manifest("res", "autosave_manifest.json");
        ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, "Manifest exported", 180);
        printf("[IDE] Manifest exported.\n");
    } else if (strcmp(command_id, "tools.paint") == 0) {
        app->current_tool = TOOL_PAINT;
    } else if (strcmp(command_id, "tools.fill") == 0) {
        app->current_tool = TOOL_FILL;
    } else if (strcmp(command_id, "tools.erase") == 0) {
        app->current_tool = TOOL_ERASE;
    } else if (strcmp(command_id, "tools.object") == 0) {
        app->current_tool = TOOL_PLACE_OBJECT;
    } else if (strcmp(command_id, "view.theme.classic") == 0) {
        apply_theme(app, THEME_CLASSIC_DARK);
    } else if (strcmp(command_id, "view.theme.blueprint") == 0) {
        apply_theme(app, THEME_BLUEPRINT_NOIR);
    } else if (strcmp(command_id, "view.theme.retro") == 0) {
        apply_theme(app, THEME_RETRO_AMBER);
    } else if (strcmp(command_id, "view.theme.contrast") == 0) {
        apply_theme(app, THEME_HIGH_CONTRAST);
    } else if (strcmp(command_id, "engine.validate.encounter") == 0) {
        EncounterBudget b;
        char reason[128];
        b.chapter = 3;
        b.recommended_party_level_min = 4;
        b.recommended_party_level_max = 6;
        b.enemy_cr_total = 18;
        game_tools_validate_encounter(&b, reason, sizeof(reason));
        printf("[IDE] Encounter validation: %s\n", reason);
    } else if (strcmp(command_id, "gb.scene.new") == 0) {
        char name[48];
        int scene_id;
        snprintf(name, sizeof(name), "Scene %d", app->gb_project.next_scene_id);
        scene_id = gbstudio_add_scene(&app->gb_project, name, 32, 24, 1);
        printf("[GB Studio] Added scene id=%d name=%s\n", scene_id, name);
    } else if (strcmp(command_id, "gb.actor.add") == 0) {
        int actor_id = gbstudio_add_actor(
            &app->gb_project,
            app->gb_project.active_scene_id,
            "Quest NPC",
            "npc_quest",
            (app->frame_count / 15) % 28,
            (app->frame_count / 17) % 20,
            1,
            false
        );
        printf("[GB Studio] Added actor id=%d in scene=%d\n", actor_id, app->gb_project.active_scene_id);
    } else if (strcmp(command_id, "gb.actor.add_hostile") == 0) {
        int actor_id = gbstudio_add_actor(
            &app->gb_project,
            app->gb_project.active_scene_id,
            "Hostile Slime",
            "enemy_slime",
            (app->frame_count / 9) % 28,
            (app->frame_count / 11) % 20,
            2,
            true
        );
        printf("[GB Studio] Added hostile actor id=%d in scene=%d\n", actor_id, app->gb_project.active_scene_id);
    } else if (strcmp(command_id, "gb.trigger.add") == 0) {
        int event_id = gbstudio_add_script_event(&app->gb_project, GB_SCRIPT_HEAL_PLAYER, 2, 0, 0);
        int trigger_id = gbstudio_add_trigger(&app->gb_project, app->gb_project.active_scene_id, "Heal Zone", 14, 11, 2, 2, event_id);
        printf("[GB Studio] Added trigger id=%d linked_event=%d\n", trigger_id, event_id);
    } else if (strcmp(command_id, "gb.script.add") == 0) {
        int event_id = gbstudio_add_script_event(&app->gb_project, GB_SCRIPT_SET_FLAG, 1, 1, 0);
        printf("[GB Studio] Added script event id=%d opcode=SET_FLAG\n", event_id);
    } else if (strcmp(command_id, "gb.script.add.reward") == 0) {
        int event_id = gbstudio_add_script_event(&app->gb_project, GB_SCRIPT_ADD_GOLD, 25, 0, 0);
        printf("[GB Studio] Added script event id=%d opcode=ADD_GOLD\n", event_id);
    } else if (strcmp(command_id, "gb.script.add.damage") == 0) {
        int event_id = gbstudio_add_script_event(&app->gb_project, GB_SCRIPT_DAMAGE_PLAYER, 3, 0, 0);
        printf("[GB Studio] Added script event id=%d opcode=DAMAGE_PLAYER\n", event_id);
    } else if (strcmp(command_id, "gb.script.run_last") == 0) {
        int last_id = app->gb_project.script_event_count > 0
            ? app->gb_project.script_events[app->gb_project.script_event_count - 1].id
            : -1;
        char result[96];
        if (gbstudio_run_script_event(&app->gb_project, last_id, result, sizeof(result))) {
            printf("[GB Studio] Script result: %s\n", result);
        } else {
            printf("[GB Studio] Script failed: %s\n", result);
        }
    } else if (strcmp(command_id, "gb.validate.scene") == 0) {
        char reason[128];
        if (gbstudio_validate_scene(&app->gb_project, app->gb_project.active_scene_id, reason, sizeof(reason))) {
            printf("[GB Studio] %s\n", reason);
        } else {
            printf("[GB Studio] Validation failed: %s\n", reason);
        }
    } else if (strcmp(command_id, "gb.validate.all") == 0) {
        char reason[128];
        if (gbstudio_validate_all_scenes(&app->gb_project, reason, sizeof(reason))) {
            printf("[GB Studio] %s\n", reason);
        } else {
            printf("[GB Studio] Validation failed: %s\n", reason);
        }
    } else if (strcmp(command_id, "gb.scene.next") == 0) {
        int i;
        for (i = 0; i < app->gb_project.scene_count; i++) {
            if (app->gb_project.scenes[i].id == app->gb_project.active_scene_id) {
                int ni = (i + 1) % app->gb_project.scene_count;
                app->gb_project.active_scene_id = app->gb_project.scenes[ni].id;
                printf("[GB Studio] Active scene: %s (%d)\n", app->gb_project.scenes[ni].name, app->gb_project.active_scene_id);
                break;
            }
        }
    } else if (strcmp(command_id, "gb.scene.prev") == 0) {
        int i;
        for (i = 0; i < app->gb_project.scene_count; i++) {
            if (app->gb_project.scenes[i].id == app->gb_project.active_scene_id) {
                int pi = (i == 0) ? (app->gb_project.scene_count - 1) : (i - 1);
                app->gb_project.active_scene_id = app->gb_project.scenes[pi].id;
                printf("[GB Studio] Active scene: %s (%d)\n", app->gb_project.scenes[pi].name, app->gb_project.active_scene_id);
                break;
            }
        }
    } else if (strcmp(command_id, "gb.play.tick") == 0) {
        gbstudio_tick_game_logic(&app->gb_project);
        printf("[GB Studio] Tick=%d HP=%d/%d Gold=%d ActiveScene=%d\n",
            app->gb_project.world_tick,
            app->gb_project.player_hp,
            app->gb_project.player_hp_max,
            app->gb_project.player_gold,
            app->gb_project.active_scene_id);
    } else if (strcmp(command_id, "gb.play.toggle") == 0) {
        app->gb_play_mode = !app->gb_play_mode;
        ui_api_set_widget_enabled(&app->ui, "widget.toolbar.play", !app->gb_play_mode);
        ui_api_set_widget_enabled(&app->ui, "widget.toolbar.stop", app->gb_play_mode);
        printf("[GB Studio] Play mode: %s\n", app->gb_play_mode ? "ON" : "OFF");
    } else if (strcmp(command_id, "gb.project.save") == 0) {
        if (gbstudio_save_project(&app->gb_project, "gbstudio_project.db")) {
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, "GB project saved", 180);
            printf("[GB Studio] Project saved to gbstudio_project.db\n");
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_ERROR, "GB project save failed", 240);
            printf("[GB Studio] Project save failed\n");
        }
    } else if (strcmp(command_id, "gb.project.load") == 0) {
        if (gbstudio_load_project(&app->gb_project, "gbstudio_project.db")) {
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, "GB project loaded", 180);
            printf("[GB Studio] Project loaded from gbstudio_project.db\n");
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_ERROR, "GB project load failed", 240);
            printf("[GB Studio] Project load failed\n");
        }
    } else if (strcmp(command_id, "source.scan") == 0) {
        source_workspace_scan(&app->sources);
        source_workspace_open_active(&app->sources);
        ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->sources.last_status, 180);
        printf("[Source] %s\n", app->sources.last_status);
    } else if (strcmp(command_id, "source.next") == 0) {
        if (source_workspace_next(&app->sources) && source_workspace_open_active(&app->sources)) {
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->sources.last_status, 180);
            printf("[Source] %s\n", app->sources.last_status);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, "No next source file", 180);
        }
    } else if (strcmp(command_id, "source.prev") == 0) {
        if (source_workspace_prev(&app->sources) && source_workspace_open_active(&app->sources)) {
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->sources.last_status, 180);
            printf("[Source] %s\n", app->sources.last_status);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, "No previous source file", 180);
        }
    } else if (strcmp(command_id, "source.open") == 0) {
        if (source_workspace_open_active(&app->sources)) {
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->sources.last_status, 180);
            printf("[Source] %s\n", app->sources.last_status);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, "Open source failed", 180);
        }
    } else if (strcmp(command_id, "source.save") == 0) {
        if (source_workspace_save_active(&app->sources)) {
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->sources.last_status, 180);
            printf("[Source] %s\n", app->sources.last_status);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, "Save source failed", 180);
        }
    } else if (strcmp(command_id, "source.compile.active") == 0) {
        if (source_workspace_compile_active(&app->sources, "auto")) {
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->sources.last_status, 180);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_ERROR, app->sources.last_status, 240);
        }
        printf("[Source] %s\n", app->sources.last_status);
    } else if (strcmp(command_id, "source.compile.asm") == 0) {
        if (source_workspace_compile_active(&app->sources, "asm")) {
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->sources.last_status, 180);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_ERROR, app->sources.last_status, 240);
        }
        printf("[Source] %s\n", app->sources.last_status);
    } else if (strcmp(command_id, "source.compile.c") == 0) {
        if (source_workspace_compile_active(&app->sources, "c")) {
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->sources.last_status, 180);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_ERROR, app->sources.last_status, 240);
        }
        printf("[Source] %s\n", app->sources.last_status);
    }
}

static void process_hotkeys(IDEEditorApp *app, int ch) {
    ToolPoint p;
    const char *menu_cmd = NULL;
    p.x = (app->frame_count / 5) % (app->active_map ? app->active_map->width : 1);
    p.y = (app->frame_count / 7) % (app->active_map ? app->active_map->height : 1);

    static BuildOutputBuffer build_output;
    static int show_build_output = 0;
    static int show_settings = 0;
    static int show_help = 0;
    static int show_about = 0;
    static SettingsUI *settings_ui = NULL;

    if (ch == 9) {
        menu_interaction_toggle(&app->interactive_menu);
        return;
    }

    if (app->interactive_menu.active) {
        if (ch == KEY_LEFT) menu_interaction_move_left(&app->interactive_menu);
        else if (ch == KEY_RIGHT) menu_interaction_move_right(&app->interactive_menu);
        else if (ch == KEY_UP) menu_interaction_move_up(&app->interactive_menu);
        else if (ch == KEY_DOWN) menu_interaction_move_down(&app->interactive_menu);
        else if (ch == 13) {
            menu_cmd = menu_interaction_activate(&app->interactive_menu);
            if (menu_cmd) execute_command(app, menu_cmd);
        } else if (ch == 27) {
            menu_interaction_close(&app->interactive_menu);
        }
        return;
    }

    switch (ch) {
        case '1': execute_command(app, "tools.paint"); break;
        case '2': execute_command(app, "tools.fill"); break;
        case '3': execute_command(app, "tools.erase"); break;
        case '4': execute_command(app, "tools.object"); break;
        case 'u': case 'U': execute_command(app, "edit.undo"); break;
        case 'r': case 'R': execute_command(app, "edit.redo"); break;
        case 's': case 'S': execute_command(app, "file.save_project"); break;
        case 'o': case 'O': execute_command(app, "file.open_project"); break;
        case 'm': case 'M': execute_command(app, "file.export_manifest"); break;
        case 't': case 'T':
            apply_theme(app, (EditorThemeId)((app->theme_id + 1) % THEME_COUNT));
            break;
        case 'p': case 'P':
            tool_apply(app->current_tool, app->active_map, &app->commands, p, app->selected_tile_id);
            break;
        case 'g': case 'G': execute_command(app, "gb.scene.new"); break;
        case 'h': case 'H': execute_command(app, "gb.actor.add"); break;
        case 'j': case 'J': execute_command(app, "gb.trigger.add"); break;
        case 'k': case 'K': execute_command(app, "gb.script.run_last"); break;
        case 'v': case 'V': execute_command(app, "gb.validate.scene"); break;
        case 'b': case 'B': execute_command(app, "gb.validate.all"); break;
        case 'n': case 'N': execute_command(app, "gb.scene.next"); break;
        case 'x': case 'X': execute_command(app, "gb.scene.prev"); break;
        case 'l': case 'L': execute_command(app, "gb.play.toggle"); break;
        case 'y': case 'Y': execute_command(app, "gb.project.save"); break;
        case 'z': case 'Z': execute_command(app, "gb.project.load"); break;
        case KEY_F5:
            build_run_project("build.bat", &build_output);
            show_build_output = 1;
            break;
        case KEY_F6:
            build_run_project("ide_editor_x64.exe", &build_output);
            show_build_output = 1;
            break;
        case KEY_F7:
            execute_command(app, "source.scan");
            break;
        case KEY_F8:
            execute_command(app, "source.compile.active");
            break;
        case KEY_F10:
            if (!settings_ui) settings_ui = settings_ui_create();
            show_settings = !show_settings;
            break;
        case KEY_F1:
            show_help = !show_help;
            break;
        case KEY_F2:
            show_about = !show_about;
            break;
        default:
            break;
    }
    if (show_build_output) {
        build_output_draw(&build_output);
        show_build_output = 0;
    }
    if (show_settings && settings_ui) {
        settings_ui_draw(settings_ui);
        show_settings = 0;
    }
    if (show_help) {
        show_help_screen();
        show_help = 0;
    }
    if (show_about) {
        show_about_screen();
        show_about = 0;
    }
}

IDEEditorApp *ide_editor_create_with_backend(EditorBackendMode mode) {
    IDEEditorApp *app = (IDEEditorApp *)malloc(sizeof(IDEEditorApp));
    if (!app) return NULL;

    app->main_window = NULL;
    app->backend = editor_backend_create(mode, "IDE Editor", 1280, 800);
    if (!app->backend) {
        free(app);
        return NULL;
    }
    app->top_menu = menu_create();
    app->sidebar = sidebar_create();
    app->widget_count = 0;

    app->project_db = project_db_create();
    app->active_map = tilemap_create(64, 48, 0);
    command_stack_init(&app->commands);
    app->current_tool = TOOL_PAINT;
    app->selected_tile_id = 7;

    editor_settings_init(&app->settings);
    app->theme_id = THEME_BLUEPRINT_NOIR;
    ui_api_init(&app->ui, app->theme_id);
    editor_tools_init(&app->tool_registry);
    command_registry_init(&app->command_registry);
    menu_interaction_init(&app->interactive_menu);
    gbstudio_project_init(&app->gb_project);

    app->frame_count = 0;
    app->autosave_enabled = true;
    app->mouse_menu_open = false;
    app->mouse_menu_top = -1;
    app->gb_play_mode = false;
    source_workspace_init(&app->sources, "..\\src");

    register_commands(app);
    build_menus(app);
    menu_interaction_build(&app->interactive_menu, app->top_menu);
    build_ui_layout(app);
    build_sidebar(app);
    register_tools(app);
    editor_seed_default_content(app);
    gbstudio_seed_default_content(app);
    editor_demo_apply_tools(app);
    source_workspace_scan(&app->sources);
    source_workspace_open_active(&app->sources);

    editor_settings_add_recent(&app->settings, "autosave_project.db");
    return app;
}

IDEEditorApp *ide_editor_create(void) {
    return ide_editor_create_with_backend(EDITOR_BACKEND_CONSOLE);
}

void ide_editor_run(IDEEditorApp *app) {
    int max_frames = 60 * 60 * 4;
    const char *exit_reason = "unknown";
    if (!app) return;

    printf("[IDE] Unreal-style editor shell ready for 2D Game Boy workflows.\n");
    printf("[IDE] Backend: %s\n", editor_backend_name(app->backend));
    printf("[IDE] Menus: %d, Sidebar sections: %d, Assets: %d, Tools: %d\n",
        menu_item_count(app->top_menu),
        sidebar_item_count(app->sidebar),
        app->project_db ? app->project_db->asset_count : 0,
        app->tool_registry.count
    );
    printf("[IDE] GB Studio: scenes=%d actors=%d triggers=%d scripts=%d\n",
        app->gb_project.scene_count,
        app->gb_project.actor_count,
        app->gb_project.trigger_count,
        app->gb_project.script_event_count);
    printf("[IDE] GUI API: panels=%d widgets=%d menuItems=%d\n",
        app->ui.panel_count,
        app->ui.widget_count,
        app->ui.menu_item_count);
    printf("[IDE] Hotkeys: 1/2/3/4 tools, P apply, U undo, R redo, S save, O open, T theme,\n");
    printf("      G/H/J/K/V GB authoring, B validate-all, N/X scene nav, L play mode, Y save, Z load, Q/Esc quit.\n");
    ui_api_render_unreal_layout(&app->ui);

    while (editor_backend_is_running(app->backend) && app->frame_count < max_frames) {
        char overlay[16384];
        EditorUIAction actions[EDITOR_UI_MAX_ACTIONS];
        int action_count = 0;
        char clicked_cmd[64];
        build_overlay_text(app, overlay, (int)sizeof(overlay));
        build_ui_actions(app, actions, &action_count, EDITOR_UI_MAX_ACTIONS);
        editor_backend_set_overlay(app->backend, overlay);
        editor_backend_set_ui_actions(app->backend, actions, action_count);

        editor_backend_poll(app->backend);
        editor_backend_render(app->backend);
        while (editor_backend_pop_ui_action(app->backend, clicked_cmd, (int)sizeof(clicked_cmd))) {
            handle_ui_action(app, clicked_cmd);
        }
        if (app->gb_play_mode) {
            gbstudio_tick_game_logic(&app->gb_project);
        }
        ui_api_tick(&app->ui);

        if (_kbhit()) {
            int ch = _getch();
            if (ch == 0 || ch == 224) {
                int ext = _getch();
                ch = KEY_EXT_BASE + ext;
            }
            if (!app->interactive_menu.active && (ch == 27 || ch == 'q' || ch == 'Q')) {
                exit_reason = "user_quit_key";
                break;
            }
            process_hotkeys(app, ch);
        }

        if (app->autosave_enabled && app->frame_count > 0 &&
            (app->frame_count % (app->settings.project.autosave_seconds * 60) == 0)) {
            project_io_save("autosave_project.db", app->project_db, app->active_map);
        }

        Sleep(16);
        app->frame_count++;
    }
    if (strcmp(exit_reason, "unknown") == 0) {
        if (!editor_backend_is_running(app->backend)) {
            exit_reason = "backend_closed";
        } else if (app->frame_count >= max_frames) {
            exit_reason = "frame_limit";
        } else {
            exit_reason = "loop_ended";
        }
    }

    if (app->active_map && app->autosave_enabled) {
        tilemap_save_csv(app->active_map, "autosave_tilemap.csv");
    }
    asset_tools_generate_manifest("res", "autosave_manifest.json");
    project_io_save("autosave_project.db", app->project_db, app->active_map);
    editor_settings_save(&app->settings, "editor_settings.ini");
    ui_api_save_layout(&app->ui, "editor_ui_layout.ini");

    {
        FILE *f = fopen("ide_editor_run.log", "a");
        if (f) {
            fprintf(f, "session_end frames=%d reason=%s backend=%s\n",
                app->frame_count, exit_reason, editor_backend_name(app->backend));
            fclose(f);
        }
    }
    printf("[IDE] Session ended after %d frames. reason=%s\n", app->frame_count, exit_reason);
}

void ide_editor_destroy(IDEEditorApp *app) {
    int i;
    if (!app) return;

    editor_backend_destroy(app->backend);
    menu_destroy(app->top_menu);
    sidebar_destroy(app->sidebar);
    for (i = 0; i < app->widget_count; i++) {
        widget_destroy(app->widgets[i]);
    }
    source_workspace_free(&app->sources);
    tilemap_destroy(app->active_map);
    project_db_destroy(app->project_db);
    free(app);
}
