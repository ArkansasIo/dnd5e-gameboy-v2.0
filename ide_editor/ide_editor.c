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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

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
}

static void build_menus(IDEEditorApp *app) {
    menu_add_item(app->top_menu, "File/New Project", "file.new_project");
    menu_add_item(app->top_menu, "File/Open Project", "file.open_project");
    menu_add_item(app->top_menu, "File/Save Project", "file.save_project");
    menu_add_item(app->top_menu, "File/Save All", "file.save_all");
    menu_add_item(app->top_menu, "File/Import/Asset", "file.import_asset");
    menu_add_item(app->top_menu, "File/Export/Manifest", "file.export_manifest");

    menu_add_item(app->top_menu, "Edit/Undo", "edit.undo");
    menu_add_item(app->top_menu, "Edit/Redo", "edit.redo");

    menu_add_item(app->top_menu, "View/Themes/Classic Dark", "view.theme.classic");
    menu_add_item(app->top_menu, "View/Themes/Blueprint Noir", "view.theme.blueprint");
    menu_add_item(app->top_menu, "View/Themes/Retro Amber", "view.theme.retro");
    menu_add_item(app->top_menu, "View/Themes/High Contrast", "view.theme.contrast");

    menu_add_item(app->top_menu, "Tools/Terrain/Paint", "tools.paint");
    menu_add_item(app->top_menu, "Tools/Terrain/Fill", "tools.fill");
    menu_add_item(app->top_menu, "Tools/Terrain/Erase", "tools.erase");
    menu_add_item(app->top_menu, "Tools/Objects/Place", "tools.object");

    menu_add_item(app->top_menu, "Engine/Validate Encounter", "engine.validate.encounter");
    menu_add_item(app->top_menu, "Engine/Rebuild Worldgen", "engine.rebuild.worldgen");

    menu_add_item(app->top_menu, "GB Studio/Scenes/New Scene", "gb.scene.new");
    menu_add_item(app->top_menu, "GB Studio/Actors/Add NPC", "gb.actor.add");
    menu_add_item(app->top_menu, "GB Studio/Actors/Add Hostile", "gb.actor.add_hostile");
    menu_add_item(app->top_menu, "GB Studio/Triggers/Add Trigger", "gb.trigger.add");
    menu_add_item(app->top_menu, "GB Studio/Scripts/New Script Event", "gb.script.add");
    menu_add_item(app->top_menu, "GB Studio/Scripts/New Reward Script", "gb.script.add.reward");
    menu_add_item(app->top_menu, "GB Studio/Scripts/New Damage Script", "gb.script.add.damage");
    menu_add_item(app->top_menu, "GB Studio/Scripts/Run Last Script Event", "gb.script.run_last");
    menu_add_item(app->top_menu, "GB Studio/Scenes/Next Scene", "gb.scene.next");
    menu_add_item(app->top_menu, "GB Studio/Scenes/Previous Scene", "gb.scene.prev");
    menu_add_item(app->top_menu, "GB Studio/Build/Validate Active Scene", "gb.validate.scene");
    menu_add_item(app->top_menu, "GB Studio/Build/Validate All Scenes", "gb.validate.all");
    menu_add_item(app->top_menu, "GB Studio/Play/Single Tick", "gb.play.tick");
    menu_add_item(app->top_menu, "GB Studio/Play/Toggle Play Mode", "gb.play.toggle");
    menu_add_item(app->top_menu, "GB Studio/Project/Save", "gb.project.save");
    menu_add_item(app->top_menu, "GB Studio/Project/Load", "gb.project.load");
}

static void build_ui_layout(IDEEditorApp *app) {
    ui_api_add_panel(&app->ui, "panel.hierarchy", "World Hierarchy", UI_DOCK_LEFT);
    ui_api_add_panel(&app->ui, "panel.outliner", "Asset Outliner", UI_DOCK_LEFT);
    ui_api_add_panel(&app->ui, "panel.viewport", "2D Viewport", UI_DOCK_CENTER);
    ui_api_add_panel(&app->ui, "panel.details", "Details", UI_DOCK_RIGHT);
    ui_api_add_panel(&app->ui, "panel.content", "Content Browser", UI_DOCK_BOTTOM);
    ui_api_add_panel(&app->ui, "panel.console", "Output Log", UI_DOCK_BOTTOM);
    ui_api_add_panel(&app->ui, "panel.blueprint", "Blueprint Graph", UI_DOCK_CENTER);
    ui_api_add_panel(&app->ui, "panel.profiler", "Perf/Memory", UI_DOCK_RIGHT);
}

static void build_sidebar(IDEEditorApp *app) {
    sidebar_add_item(app->sidebar, "Project Explorer");
    sidebar_add_item(app->sidebar, "World Database");
    sidebar_add_item(app->sidebar, "Tilemap Layers");
    sidebar_add_item(app->sidebar, "Object Palette");
    sidebar_add_item(app->sidebar, "Blueprint Tools");
    sidebar_add_item(app->sidebar, "Build and Deploy");
    sidebar_add_item(app->sidebar, "Debug and Validation");
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
    app->theme_id = id;
    ui_api_apply_theme(&app->ui, id);
    printf("[IDE] Theme switched: %s\n", editor_theme_name(id));
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
        printf("[IDE] Project and settings saved.\n");
    } else if (strcmp(command_id, "file.open_project") == 0) {
        project_io_load("autosave_project.db", app->project_db);
        editor_settings_load(&app->settings, "editor_settings.ini");
        printf("[IDE] Project and settings loaded.\n");
    } else if (strcmp(command_id, "file.export_manifest") == 0) {
        asset_tools_generate_manifest("res", "autosave_manifest.json");
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
        printf("[GB Studio] Play mode: %s\n", app->gb_play_mode ? "ON" : "OFF");
    } else if (strcmp(command_id, "gb.project.save") == 0) {
        if (gbstudio_save_project(&app->gb_project, "gbstudio_project.db")) {
            printf("[GB Studio] Project saved to gbstudio_project.db\n");
        } else {
            printf("[GB Studio] Project save failed\n");
        }
    } else if (strcmp(command_id, "gb.project.load") == 0) {
        if (gbstudio_load_project(&app->gb_project, "gbstudio_project.db")) {
            printf("[GB Studio] Project loaded from gbstudio_project.db\n");
        } else {
            printf("[GB Studio] Project load failed\n");
        }
    }
}

static void process_hotkeys(IDEEditorApp *app, int ch) {
    ToolPoint p;
    p.x = (app->frame_count / 5) % (app->active_map ? app->active_map->width : 1);
    p.y = (app->frame_count / 7) % (app->active_map ? app->active_map->height : 1);

    static BuildOutputBuffer build_output;
    static int show_build_output = 0;
    static int show_settings = 0;
    static int show_help = 0;
    static int show_about = 0;
    static SettingsUI *settings_ui = NULL;
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
        case 0x3F: /* F5 */
            build_run_project("build.bat", &build_output);
            show_build_output = 1;
            break;
        case 0x40: /* F6 */
            build_run_project("ide_editor.exe", &build_output);
            show_build_output = 1;
            break;
        case 0x44: /* F10 */
            if (!settings_ui) settings_ui = settings_ui_create();
            show_settings = !show_settings;
            break;
        case 0x3B: /* F1 */
            show_help = !show_help;
            break;
        case 0x3C: /* F2 */
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
    gbstudio_project_init(&app->gb_project);

    app->frame_count = 0;
    app->autosave_enabled = true;
    app->gb_play_mode = false;

    register_commands(app);
    build_menus(app);
    build_ui_layout(app);
    build_sidebar(app);
    register_tools(app);
    editor_seed_default_content(app);
    gbstudio_seed_default_content(app);
    editor_demo_apply_tools(app);

    editor_settings_add_recent(&app->settings, "autosave_project.db");
    return app;
}

IDEEditorApp *ide_editor_create(void) {
    return ide_editor_create_with_backend(EDITOR_BACKEND_CONSOLE);
}

void ide_editor_run(IDEEditorApp *app) {
    int max_frames = 60 * 60 * 4;
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
    printf("[IDE] Hotkeys: 1/2/3/4 tools, P apply, U undo, R redo, S save, O open, T theme,\n");
    printf("      G/H/J/K/V GB authoring, B validate-all, N/X scene nav, L play mode, Y save, Z load, Q/Esc quit.\n");

    while (editor_backend_is_running(app->backend) && app->frame_count < max_frames) {
        editor_backend_poll(app->backend);
        editor_backend_render(app->backend);
        if (app->gb_play_mode) {
            gbstudio_tick_game_logic(&app->gb_project);
        }

        if (_kbhit()) {
            int ch = _getch();
            if (ch == 27 || ch == 'q' || ch == 'Q') break;
            process_hotkeys(app, ch);
        }

        if (app->autosave_enabled && app->frame_count > 0 &&
            (app->frame_count % (app->settings.project.autosave_seconds * 60) == 0)) {
            project_io_save("autosave_project.db", app->project_db, app->active_map);
        }

        Sleep(16);
        app->frame_count++;
    }

    if (app->active_map && app->autosave_enabled) {
        tilemap_save_csv(app->active_map, "autosave_tilemap.csv");
    }
    asset_tools_generate_manifest("res", "autosave_manifest.json");
    project_io_save("autosave_project.db", app->project_db, app->active_map);
    editor_settings_save(&app->settings, "editor_settings.ini");

    printf("[IDE] Session ended after %d frames.\n", app->frame_count);
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
    tilemap_destroy(app->active_map);
    project_db_destroy(app->project_db);
    free(app);
}
