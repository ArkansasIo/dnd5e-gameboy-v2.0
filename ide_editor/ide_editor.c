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
#include "feature_autogen/auto_feature_registry.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>

#define KEY_EXT_BASE 1000
#define KEY_UP (KEY_EXT_BASE + 72)
#define KEY_DOWN (KEY_EXT_BASE + 80)
#define KEY_LEFT (KEY_EXT_BASE + 75)
#define KEY_RIGHT (KEY_EXT_BASE + 77)
#define KEY_F1 (KEY_EXT_BASE + 59)
#define KEY_F2 (KEY_EXT_BASE + 60)
#define KEY_F3 (KEY_EXT_BASE + 61)
#define KEY_F4 (KEY_EXT_BASE + 62)
#define KEY_F5 (KEY_EXT_BASE + 63)
#define KEY_F6 (KEY_EXT_BASE + 64)
#define KEY_F7 (KEY_EXT_BASE + 65)
#define KEY_F8 (KEY_EXT_BASE + 66)
#define KEY_F9 (KEY_EXT_BASE + 67)
#define KEY_F10 (KEY_EXT_BASE + 68)

static auto_feature_registry g_auto_features;

static void register_commands(IDEEditorApp *app) {
    command_registry_add(&app->command_registry, "file.new_project", "Create a new project workspace");
    command_registry_add(&app->command_registry, "file.open_project", "Open existing project file");
    command_registry_add(&app->command_registry, "file.save_project", "Save project file");
    command_registry_add(&app->command_registry, "file.save_all", "Save all open assets and maps");
    command_registry_add(&app->command_registry, "file.import_asset", "Import external asset");
    command_registry_add(&app->command_registry, "file.import_map_data", "Import tilemap CSV data");
    command_registry_add(&app->command_registry, "file.export_manifest", "Generate asset manifest");
    command_registry_add(&app->command_registry, "file.export.build_package", "Build export package");

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
    command_registry_add(&app->command_registry, "view.theme.vscode.dark_plus", "Switch to VS Code Dark+ theme");
    command_registry_add(&app->command_registry, "view.theme.vscode.light_plus", "Switch to VS Code Light+ theme");
    command_registry_add(&app->command_registry, "view.theme.vscode.monokai", "Switch to VS Code Monokai theme");
    command_registry_add(&app->command_registry, "view.theme.vscode.solarized_dark", "Switch to VS Code Solarized Dark theme");
    command_registry_add(&app->command_registry, "view.theme.vscode.solarized_light", "Switch to VS Code Solarized Light theme");
    command_registry_add(&app->command_registry, "view.theme.vscode.abyss", "Switch to VS Code Abyss theme");
    command_registry_add(&app->command_registry, "view.theme.vscode.kimbie_dark", "Switch to VS Code Kimbie Dark theme");
    command_registry_add(&app->command_registry, "view.theme.vscode.red", "Switch to VS Code Red theme");
    command_registry_add(&app->command_registry, "view.theme.vscode.quiet_light", "Switch to VS Code Quiet Light theme");
    command_registry_add(&app->command_registry, "edit.duplicate_selection", "Duplicate a sample tile selection");
    command_registry_add(&app->command_registry, "edit.delete_selection", "Delete a sample tile selection");
    command_registry_add(&app->command_registry, "editor.preferences.open", "Open editor preferences");
    command_registry_add(&app->command_registry, "view.panel.world_hierarchy", "Toggle World Hierarchy panel");
    command_registry_add(&app->command_registry, "view.panel.asset_outliner", "Toggle Asset Outliner panel");
    command_registry_add(&app->command_registry, "view.panel.details", "Toggle Details panel");
    command_registry_add(&app->command_registry, "view.panel.output_log", "Toggle Output Log panel");
    command_registry_add(&app->command_registry, "view.panel.profiler", "Toggle Profiler panel");
    command_registry_add(&app->command_registry, "window.layout.load_unreal_gb", "Load Unreal GB layout");
    command_registry_add(&app->command_registry, "window.layout.save_current", "Save current layout");
    command_registry_add(&app->command_registry, "window.layout.reset", "Reset layout visibility");
    command_registry_add(&app->command_registry, "help.quick_start", "Show quick start information");
    command_registry_add(&app->command_registry, "help.controls", "Show controls reference");
    command_registry_add(&app->command_registry, "help.about", "Show about screen");

    command_registry_add(&app->command_registry, "engine.validate.encounter", "Validate encounter budget");
    command_registry_add(&app->command_registry, "engine.rebuild.worldgen", "Trigger worldgen rebuild hook");
    command_registry_add(&app->command_registry, "engine.runtime.tick", "Advance runtime simulation by one turn");
    command_registry_add(&app->command_registry, "engine.runtime.spawn_enemy", "Spawn a hostile runtime entity");
    command_registry_add(&app->command_registry, "engine.runtime.attack", "Player attacks nearest hostile entity");
    command_registry_add(&app->command_registry, "engine.quest.next", "Switch active runtime quest");
    command_registry_add(&app->command_registry, "engine.quest.progress", "Increment active runtime quest progress");
    command_registry_add(&app->command_registry, "engine.quest.complete", "Complete active runtime quest when ready");
    command_registry_add(&app->command_registry, "blueprint.new_graph", "Create a new blueprint graph");
    command_registry_add(&app->command_registry, "blueprint.compile_graph", "Compile the active blueprint graph");
    command_registry_add(&app->command_registry, "blueprint.validate_graph", "Validate the active blueprint graph");

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
    command_registry_add(&app->command_registry, "gb.play.stop", "Stop GB play mode");
    command_registry_add(&app->command_registry, "gb.project.save", "Save GB Studio project data");
    command_registry_add(&app->command_registry, "gb.project.load", "Load GB Studio project data");
    command_registry_add(&app->command_registry, "gb.project.export_rom_config", "Export GB Studio ROM config");

    command_registry_add(&app->command_registry, "source.scan", "Scan source folder");
    command_registry_add(&app->command_registry, "source.next", "Next source file");
    command_registry_add(&app->command_registry, "source.prev", "Previous source file");
    command_registry_add(&app->command_registry, "source.open", "Open active source file");
    command_registry_add(&app->command_registry, "source.save", "Save active source file");
    command_registry_add(&app->command_registry, "source.compile.active", "Compile active source file");
    command_registry_add(&app->command_registry, "source.compile.asm", "Compile active source as assembly");
    command_registry_add(&app->command_registry, "source.compile.c", "Compile active source as C");
    command_registry_add(&app->command_registry, "source.insert.stub", "Insert TODO stub line at top of active source");
    command_registry_add(&app->command_registry, "source.delete.line", "Delete current source line");
    command_registry_add(&app->command_registry, "source.search.includes", "Find next include line in active source");

    command_registry_add(&app->command_registry, "editor.command_palette.toggle", "Toggle command palette");
    command_registry_add(&app->command_registry, "editor.command_palette.next", "Select next command in palette");
    command_registry_add(&app->command_registry, "editor.command_palette.prev", "Select previous command in palette");
    command_registry_add(&app->command_registry, "editor.command_palette.run", "Run selected command palette item");

    {
        int i;
        int n = 0;
        const auto_feature_descriptor *defs = auto_feature_descriptors(&n);
        for (i = 0; i < n; ++i) {
            char cmd_enable[96];
            char cmd_disable[96];
            char desc_enable[128];
            char desc_disable[128];
            snprintf(cmd_enable, sizeof(cmd_enable), "%s.enable", defs[i].id);
            snprintf(cmd_disable, sizeof(cmd_disable), "%s.disable", defs[i].id);
            snprintf(desc_enable, sizeof(desc_enable), "Enable module: %s", defs[i].title);
            snprintf(desc_disable, sizeof(desc_disable), "Disable module: %s", defs[i].title);
            command_registry_add(&app->command_registry, cmd_enable, desc_enable);
            command_registry_add(&app->command_registry, cmd_disable, desc_disable);
        }
    }
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
    add_menu(app, "File/Import/Map Data", "file.import_map_data");
    add_menu(app, "File/Source/Scan src", "source.scan");
    add_menu(app, "File/Source/Next File", "source.next");
    add_menu(app, "File/Source/Previous File", "source.prev");
    add_menu(app, "File/Source/Open Active", "source.open");
    add_menu(app, "File/Source/Save Active", "source.save");
    add_menu(app, "File/Source/Insert Stub Line", "source.insert.stub");
    add_menu(app, "File/Source/Delete Current Line", "source.delete.line");
    add_menu(app, "File/Source/Search/Next Include", "source.search.includes");
    add_menu(app, "File/Export/Manifest", "file.export_manifest");
    add_menu(app, "File/Export/Build Package", "file.export.build_package");

    add_menu(app, "Edit/Undo", "edit.undo");
    add_menu(app, "Edit/Redo", "edit.redo");
    add_menu(app, "Edit/Duplicate Selection", "edit.duplicate_selection");
    add_menu(app, "Edit/Delete Selection", "edit.delete_selection");
    add_menu(app, "Edit/Editor Preferences", "editor.preferences.open");

    add_menu(app, "View/Themes/Classic Dark", "view.theme.classic");
    add_menu(app, "View/Themes/Blueprint Noir", "view.theme.blueprint");
    add_menu(app, "View/Themes/Retro Amber", "view.theme.retro");
    add_menu(app, "View/Themes/High Contrast", "view.theme.contrast");
    add_menu(app, "View/Themes/VS Code/Dark+", "view.theme.vscode.dark_plus");
    add_menu(app, "View/Themes/VS Code/Light+", "view.theme.vscode.light_plus");
    add_menu(app, "View/Themes/VS Code/Monokai", "view.theme.vscode.monokai");
    add_menu(app, "View/Themes/VS Code/Solarized Dark", "view.theme.vscode.solarized_dark");
    add_menu(app, "View/Themes/VS Code/Solarized Light", "view.theme.vscode.solarized_light");
    add_menu(app, "View/Themes/VS Code/Abyss", "view.theme.vscode.abyss");
    add_menu(app, "View/Themes/VS Code/Kimbie Dark", "view.theme.vscode.kimbie_dark");
    add_menu(app, "View/Themes/VS Code/Red", "view.theme.vscode.red");
    add_menu(app, "View/Themes/VS Code/Quiet Light", "view.theme.vscode.quiet_light");
    add_menu(app, "View/Panels/World Hierarchy", "view.panel.world_hierarchy");
    add_menu(app, "View/Panels/Asset Outliner", "view.panel.asset_outliner");
    add_menu(app, "View/Panels/Details", "view.panel.details");
    add_menu(app, "View/Panels/Output Log", "view.panel.output_log");
    add_menu(app, "View/Panels/Profiler", "view.panel.profiler");

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
    add_menu(app, "Tools/Runtime/Tick", "engine.runtime.tick");
    add_menu(app, "Tools/Runtime/Spawn Enemy", "engine.runtime.spawn_enemy");
    add_menu(app, "Tools/Runtime/Player Attack", "engine.runtime.attack");
    add_menu(app, "Tools/Runtime/Quest/Next", "engine.quest.next");
    add_menu(app, "Tools/Runtime/Quest/Progress +1", "engine.quest.progress");
    add_menu(app, "Tools/Runtime/Quest/Complete", "engine.quest.complete");

    add_menu(app, "Blueprint/New Graph", "blueprint.new_graph");
    add_menu(app, "Blueprint/Compile Graph", "blueprint.compile_graph");
    add_menu(app, "Blueprint/Validate Graph", "blueprint.validate_graph");

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
    add_menu(app, "GB Studio/Play/Stop", "gb.play.stop");
    add_menu(app, "GB Studio/Build/Validate Active Scene", "gb.validate.scene");
    add_menu(app, "GB Studio/Build/Validate All Scenes", "gb.validate.all");
    add_menu(app, "GB Studio/Project/Save", "gb.project.save");
    add_menu(app, "GB Studio/Project/Load", "gb.project.load");
    add_menu(app, "GB Studio/Project/Export ROM Config", "gb.project.export_rom_config");

    add_menu(app, "Window/Layout/Load Unreal GB Preset", "window.layout.load_unreal_gb");
    add_menu(app, "Window/Layout/Save Current Layout", "window.layout.save_current");
    add_menu(app, "Window/Layout/Reset Layout", "window.layout.reset");
    add_menu(app, "Window/Command Palette/Toggle", "editor.command_palette.toggle");
    add_menu(app, "Window/Command Palette/Next", "editor.command_palette.next");
    add_menu(app, "Window/Command Palette/Previous", "editor.command_palette.prev");
    add_menu(app, "Window/Command Palette/Run Selected", "editor.command_palette.run");

    add_menu(app, "Help/Quick Start", "help.quick_start");
    add_menu(app, "Help/Controls", "help.controls");
    add_menu(app, "Help/About", "help.about");
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

static void reset_project_workspace(IDEEditorApp *app) {
    if (!app) return;
    if (app->active_map) tilemap_destroy(app->active_map);
    if (app->project_db) project_db_destroy(app->project_db);

    app->project_db = project_db_create();
    app->active_map = tilemap_create(64, 48, 0);
    command_stack_init(&app->commands);
    app->current_tool = TOOL_PAINT;
    app->selected_tile_id = 7;

    gbstudio_project_init(&app->gb_project);
    gbstudio_seed_default_content(app);
    editor_seed_default_content(app);
    editor_demo_apply_tools(app);

    source_workspace_scan(&app->sources);
    source_workspace_open_active(&app->sources);

    engine_runtime_init(&app->runtime);
    engine_runtime_seed_defaults(&app->runtime);
}

static int tilemap_load_csv_inplace(TileMap *map, const char *filename) {
    FILE *f;
    char line[4096];
    int y = 0;
    if (!map || !filename) return 0;
    f = fopen(filename, "r");
    if (!f) return 0;
    while (y < map->height && fgets(line, sizeof(line), f)) {
        char *tok;
        int x = 0;
        tok = strtok(line, ",\r\n");
        while (tok && x < map->width) {
            map->tiles[y][x] = atoi(tok);
            tok = strtok(NULL, ",\r\n");
            x++;
        }
        while (x < map->width) {
            map->tiles[y][x++] = 0;
        }
        y++;
    }
    fclose(f);
    if (y == 0) return 0;
    while (y < map->height) {
        int x;
        for (x = 0; x < map->width; ++x) map->tiles[y][x] = 0;
        y++;
    }
    map->dirty = true;
    return 1;
}

static void apply_theme(IDEEditorApp *app, EditorThemeId id) {
    char text[96];
    app->theme_id = id;
    ui_api_apply_theme(&app->ui, id);
    snprintf(text, sizeof(text), "Theme switched: %s", editor_theme_name(id));
    ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, text, 180);
    printf("[IDE] Theme switched: %s\n", editor_theme_name(id));
}

static void ui_info(IDEEditorApp *app, const char *msg);
static void ui_warn(IDEEditorApp *app, const char *msg);

static void toggle_panel_visibility(IDEEditorApp *app, const char *panel_id, const char *label) {
    UIPanel *p;
    char msg[128];
    if (!app || !panel_id || !label) return;
    p = ui_api_find_panel(&app->ui, panel_id);
    if (!p) {
        snprintf(msg, sizeof(msg), "Panel missing: %s", label);
        ui_warn(app, msg);
        return;
    }
    p->visible = !p->visible;
    snprintf(msg, sizeof(msg), "%s panel %s", label, p->visible ? "shown" : "hidden");
    ui_info(app, msg);
}

static void set_all_panels_visible(IDEEditorApp *app, bool visible) {
    int i;
    if (!app) return;
    for (i = 0; i < app->ui.panel_count; ++i) {
        app->ui.panels[i].visible = visible;
    }
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

static int contains_case_insensitive(const char *text, const char *pattern) {
    char t[256];
    char p[64];
    int i;
    int j;
    if (!text || !pattern || !pattern[0]) return 1;
    strncpy(t, text, sizeof(t) - 1);
    t[sizeof(t) - 1] = '\0';
    strncpy(p, pattern, sizeof(p) - 1);
    p[sizeof(p) - 1] = '\0';
    for (i = 0; t[i]; ++i) t[i] = (char)tolower((unsigned char)t[i]);
    for (j = 0; p[j]; ++j) p[j] = (char)tolower((unsigned char)p[j]);
    return strstr(t, p) != NULL;
}

static int source_find_next_line(const SourceWorkspace *ws, const char *term, int from_line) {
    int i;
    if (!ws || !ws->active_buffer || !term || !term[0]) return -1;
    if (from_line < 0) from_line = 0;
    for (i = from_line; i < ws->active_buffer->line_count; ++i) {
        if (contains_case_insensitive(ws->active_buffer->lines[i], term)) {
            return i;
        }
    }
    return -1;
}

static char tile_to_gb_char(int tile_id) {
    static const char lut[] = " .,:;ox%#@";
    int n = (int)(sizeof(lut) - 1);
    int idx = tile_id;
    if (idx < 0) idx = -idx;
    idx %= n;
    return lut[idx];
}

static void append_center_game_preview(IDEEditorApp *app, char *out, int out_len, int *pos) {
    enum { GW = 20, GH = 18 };
    char grid[GH][GW + 1];
    int i;
    int x;
    int y;
    int scene_w = app->active_map ? app->active_map->width : GW;
    int scene_h = app->active_map ? app->active_map->height : GH;
    int cam_x = 0;
    int cam_y = 0;
    int center_actor_x = 0;
    int center_actor_y = 0;
    int found_center_actor = 0;

    if (!app || !out || !pos) return;

    for (i = 0; i < app->gb_project.scene_count; ++i) {
        if (app->gb_project.scenes[i].id == app->gb_project.active_scene_id) {
            scene_w = app->gb_project.scenes[i].width;
            scene_h = app->gb_project.scenes[i].height;
            break;
        }
    }
    if (app->active_map) {
        if (scene_w > app->active_map->width) scene_w = app->active_map->width;
        if (scene_h > app->active_map->height) scene_h = app->active_map->height;
    }
    if (scene_w < 1) scene_w = GW;
    if (scene_h < 1) scene_h = GH;

    for (i = 0; i < app->gb_project.actor_count; ++i) {
        const GBActor *a = &app->gb_project.actors[i];
        if (a->scene_id == app->gb_project.active_scene_id) {
            center_actor_x = a->x;
            center_actor_y = a->y;
            found_center_actor = 1;
            break;
        }
    }
    if (!found_center_actor) {
        center_actor_x = scene_w / 2;
        center_actor_y = scene_h / 2;
    }

    cam_x = center_actor_x - (GW / 2);
    cam_y = center_actor_y - (GH / 2);
    if (cam_x < 0) cam_x = 0;
    if (cam_y < 0) cam_y = 0;
    if (cam_x + GW > scene_w) cam_x = scene_w - GW;
    if (cam_y + GH > scene_h) cam_y = scene_h - GH;
    if (cam_x < 0) cam_x = 0;
    if (cam_y < 0) cam_y = 0;

    for (y = 0; y < GH; ++y) {
        for (x = 0; x < GW; ++x) {
            int mx = cam_x + x;
            int my = cam_y + y;
            int t = 0;
            if (app->active_map && mx >= 0 && my >= 0 && mx < app->active_map->width && my < app->active_map->height) {
                t = tilemap_get(app->active_map, mx, my);
            }
            grid[y][x] = tile_to_gb_char(t);
        }
        grid[y][GW] = '\0';
    }

    for (i = 0; i < app->gb_project.trigger_count; ++i) {
        const GBTrigger *tr = &app->gb_project.triggers[i];
        int tx;
        int ty;
        if (tr->scene_id != app->gb_project.active_scene_id) continue;
        for (ty = 0; ty < tr->height; ++ty) {
            for (tx = 0; tx < tr->width; ++tx) {
                int gx = tr->x + tx - cam_x;
                int gy = tr->y + ty - cam_y;
                if (gx >= 0 && gy >= 0 && gx < GW && gy < GH) grid[gy][gx] = 't';
            }
        }
    }

    for (i = 0; i < app->gb_project.actor_count; ++i) {
        const GBActor *a = &app->gb_project.actors[i];
        int gx;
        int gy;
        if (a->scene_id != app->gb_project.active_scene_id) continue;
        gx = a->x - cam_x;
        gy = a->y - cam_y;
        if (gx >= 0 && gy >= 0 && gx < GW && gy < GH) {
            grid[gy][gx] = a->hostile ? 'E' : 'N';
        }
    }

    *pos = appendf(out, out_len, *pos, "\nCenter Viewport (GB Studio style 20x18):\n");
    *pos = appendf(out, out_len, *pos, "Camera: (%d,%d)  Scene: %dx%d\n", cam_x, cam_y, scene_w, scene_h);
    for (y = 0; y < GH; ++y) {
        *pos = appendf(out, out_len, *pos, "  |%s|\n", grid[y]);
    }
    *pos = appendf(out, out_len, *pos, "Legend: N=npc E=enemy t=trigger\n");
}

static void run_selected_palette_command(IDEEditorApp *app);

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
    pos = appendf(out, out_len, pos, "Last Command: %s\n", app->last_command[0] ? app->last_command : "(none)");
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
        if (app->sources.active_buffer) {
            int start = app->sources.active_buffer->cursor_line - 1;
            int end = app->sources.active_buffer->cursor_line + 1;
            int ln;
            if (start < 0) start = 0;
            if (end >= app->sources.active_buffer->line_count) end = app->sources.active_buffer->line_count - 1;
            pos = appendf(out, out_len, pos, "  Cursor Line: %d\n", app->sources.active_buffer->cursor_line + 1);
            for (ln = start; ln <= end && ln >= 0; ++ln) {
                const char *mark = (ln == app->sources.active_buffer->cursor_line) ? ">" : " ";
                pos = appendf(out, out_len, pos, "  %s%4d: %.72s\n", mark, ln + 1, app->sources.active_buffer->lines[ln]);
            }
        }
    }

    {
        char rt[256];
        int shown = 0;
        int i;
        engine_runtime_summary(&app->runtime, rt, (int)sizeof(rt));
        pos = appendf(out, out_len, pos, "\nRuntime Logic:\n  %s\n", rt);
        for (i = 0; i < app->runtime.entity_count && shown < 6; i++) {
            const EREntity *e = &app->runtime.entities[i];
            pos = appendf(out, out_len, pos, "  - %s hp:%d/%d (%s) %s\n",
                e->name,
                e->hp,
                e->hp_max,
                e->faction == ER_FACTION_HOSTILE ? "hostile" : (e->faction == ER_FACTION_PLAYER ? "player" : "friendly"),
                e->alive ? "alive" : "down");
            shown++;
        }
        pos = appendf(out, out_len, pos, "  Event: %s\n", app->runtime.last_event);
    }

    append_center_game_preview(app, out, out_len, &pos);

    if (app->command_palette_open) {
        int i;
        int shown = 0;
        pos = appendf(out, out_len, pos, "\nCommand Palette (F9 to toggle):\n");
        for (i = 0; i < app->command_registry.count && shown < 14; ++i) {
            const EditorCommandDef *def = &app->command_registry.defs[i];
            if (contains_case_insensitive(def->id, app->command_palette_filter) ||
                contains_case_insensitive(def->description, app->command_palette_filter)) {
                const char *mark = (shown == app->command_palette_index) ? ">" : " ";
                pos = appendf(out, out_len, pos, "  %s %-34s : %s\n", mark, def->id, def->description);
                shown++;
            }
        }
        if (shown == 0) {
            pos = appendf(out, out_len, pos, "  (no commands match filter)\n");
        }
        pos = appendf(out, out_len, pos, "  Filter: \"%s\"\n", app->command_palette_filter);
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
    ui_action_add(actions, &count, max_count, 486, toolbar_y + 4, 64, tool_h - 8, "Stop", "gb.play.stop");
    ui_action_add(actions, &count, max_count, 554, toolbar_y + 4, 88, tool_h - 8, "Validate", "gb.validate.all");
    ui_action_add(actions, &count, max_count, 646, toolbar_y + 4, 84, tool_h - 8, "Palette", "editor.command_palette.toggle");
    ui_action_add(actions, &count, max_count, 734, toolbar_y + 4, 80, tool_h - 8, "Script", "gb.script.run_last");
    ui_action_add(actions, &count, max_count, 818, toolbar_y + 4, 76, tool_h - 8, "Actor", "gb.actor.add");
    ui_action_add(actions, &count, max_count, 898, toolbar_y + 4, 86, tool_h - 8, "Trigger", "gb.trigger.add");
    ui_action_add(actions, &count, max_count, 988, toolbar_y + 4, 88, tool_h - 8, "Save All", "file.save_all");

    ui_action_add(actions, &count, max_count, right_x, 140, 120, 24, "Paint", "tools.paint");
    ui_action_add(actions, &count, max_count, right_x, 168, 120, 24, "Fill", "tools.fill");
    ui_action_add(actions, &count, max_count, right_x, 196, 120, 24, "Erase", "tools.erase");
    ui_action_add(actions, &count, max_count, right_x, 224, 120, 24, "Obj", "tools.object");
    ui_action_add(actions, &count, max_count, right_x, 270, 120, 24, "Scene+", "gb.scene.new");
    ui_action_add(actions, &count, max_count, right_x, 298, 120, 24, "Actor+", "gb.actor.add");
    ui_action_add(actions, &count, max_count, right_x, 326, 120, 24, "Trigger+", "gb.trigger.add");
    ui_action_add(actions, &count, max_count, right_x, 354, 120, 24, "ScriptRun", "gb.script.run_last");
    ui_action_add(actions, &count, max_count, right_x, 392, 120, 24, "RT Tick", "engine.runtime.tick");
    ui_action_add(actions, &count, max_count, right_x, 420, 120, 24, "RT Attack", "engine.runtime.attack");
    ui_action_add(actions, &count, max_count, right_x, 448, 120, 24, "Quest+1", "engine.quest.progress");
    ui_action_add(actions, &count, max_count, right_x, 476, 120, 24, "Quest Done", "engine.quest.complete");

    *out_count = count;
}

static const EditorCommandDef *palette_command_at(const IDEEditorApp *app, int visible_index) {
    int i;
    int shown = 0;
    if (!app || visible_index < 0) return NULL;
    for (i = 0; i < app->command_registry.count; ++i) {
        const EditorCommandDef *def = &app->command_registry.defs[i];
        if (contains_case_insensitive(def->id, app->command_palette_filter) ||
            contains_case_insensitive(def->description, app->command_palette_filter)) {
            if (shown == visible_index) return def;
            shown++;
        }
    }
    return NULL;
}

static int palette_visible_count(const IDEEditorApp *app) {
    int i;
    int shown = 0;
    if (!app) return 0;
    for (i = 0; i < app->command_registry.count; ++i) {
        const EditorCommandDef *def = &app->command_registry.defs[i];
        if (contains_case_insensitive(def->id, app->command_palette_filter) ||
            contains_case_insensitive(def->description, app->command_palette_filter)) {
            shown++;
        }
    }
    return shown;
}

static void run_selected_palette_command(IDEEditorApp *app) {
    const EditorCommandDef *def;
    if (!app) return;
    def = palette_command_at(app, app->command_palette_index);
    if (def && strcmp(def->id, "editor.command_palette.run") != 0) {
        execute_command(app, def->id);
    } else {
        ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, "No command selected in palette", 160);
    }
}

static void ui_info(IDEEditorApp *app, const char *msg) {
    if (!app || !msg) return;
    ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, msg, 180);
}

static void ui_warn(IDEEditorApp *app, const char *msg) {
    if (!app || !msg) return;
    ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, msg, 220);
}

static const char *gbstudio_active_scene_name(const GBStudioProject *p) {
    int i;
    if (!p) return "Unknown Scene";
    for (i = 0; i < p->scene_count; i++) {
        if (p->scenes[i].id == p->active_scene_id) {
            return p->scenes[i].name;
        }
    }
    return "Unknown Scene";
}

static void gbstudio_start_loading_screen(IDEEditorApp *app) {
    const char *scene_name;
    if (!app) return;
    scene_name = gbstudio_active_scene_name(&app->gb_project);
    snprintf(app->gb_loading_subtitle, sizeof(app->gb_loading_subtitle), "Preparing scene \"%s\" and runtime systems...", scene_name);
    app->gb_loading_screen_active = true;
    app->gb_loading_progress_percent = 0;
    app->gb_loading_frames_left = 90;
}

static void gbstudio_update_loading_screen(IDEEditorApp *app) {
    if (!app || !app->gb_loading_screen_active) return;
    if (app->gb_loading_frames_left > 0) {
        app->gb_loading_frames_left--;
    }
    app->gb_loading_progress_percent = 100 - (app->gb_loading_frames_left * 100 / 90);
    if (app->gb_loading_progress_percent < 0) app->gb_loading_progress_percent = 0;
    if (app->gb_loading_progress_percent > 100) app->gb_loading_progress_percent = 100;
    if (app->gb_loading_frames_left <= 0) {
        app->gb_loading_screen_active = false;
        app->gb_loading_progress_percent = 100;
        ui_info(app, "GB Studio game loaded");
    }
}

static void gbstudio_stop_loading_screen(IDEEditorApp *app) {
    if (!app) return;
    app->gb_loading_screen_active = false;
    app->gb_loading_progress_percent = 0;
    app->gb_loading_frames_left = 0;
    app->gb_loading_subtitle[0] = '\0';
}

static void normalize_command_id(const char *in, char *out, int out_len) {
    int start = 0;
    int end;
    int n = 0;
    if (!out || out_len <= 0) return;
    out[0] = '\0';
    if (!in) return;

    end = (int)strlen(in);
    while (in[start] && isspace((unsigned char)in[start])) start++;
    while (end > start && isspace((unsigned char)in[end - 1])) end--;
    while (start < end && n < (out_len - 1)) {
        out[n++] = in[start++];
    }
    out[n] = '\0';
}

static const char *resolve_button_command_alias(const char *command_id) {
    if (!command_id || !command_id[0]) return command_id;
    if (_stricmp(command_id, "new") == 0) return "file.new_project";
    if (_stricmp(command_id, "open") == 0) return "file.open_project";
    if (_stricmp(command_id, "save") == 0) return "file.save_project";
    if (_stricmp(command_id, "undo") == 0) return "edit.undo";
    if (_stricmp(command_id, "redo") == 0) return "edit.redo";
    if (_stricmp(command_id, "build") == 0) return "engine.rebuild.worldgen";
    if (_stricmp(command_id, "validate") == 0) return "gb.validate.all";
    if (_stricmp(command_id, "play") == 0) return "gb.play.toggle";
    if (_stricmp(command_id, "stop") == 0) return "gb.play.stop";
    if (_stricmp(command_id, "palette") == 0) return "editor.command_palette.toggle";
    if (_stricmp(command_id, "script") == 0) return "gb.script.run_last";
    if (_stricmp(command_id, "actor") == 0) return "gb.actor.add";
    if (_stricmp(command_id, "trigger") == 0) return "gb.trigger.add";
    return command_id;
}

static void handle_ui_action(IDEEditorApp *app, const char *command_id) {
    char normalized[128];
    const char *resolved;
    if (!app || !command_id) return;
    normalize_command_id(command_id, normalized, (int)sizeof(normalized));
    resolved = resolve_button_command_alias(normalized);
    if (!resolved || !resolved[0]) return;
    printf("[UI] command=%s\n", resolved);
    if (strncmp(resolved, "ui.menu.top.", 12) == 0) {
        int idx = atoi(resolved + 12);
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
    strncpy(app->last_command, resolved, sizeof(app->last_command) - 1);
    app->last_command[sizeof(app->last_command) - 1] = '\0';
    execute_command(app, resolved);
    app->mouse_menu_open = false;
    app->mouse_menu_top = -1;
}

static void execute_command(IDEEditorApp *app, const char *command_id) {
    if (!app || !command_id) return;
    if (auto_feature_registry_dispatch(&g_auto_features, command_id)) {
        char reason[128];
        if (auto_feature_registry_validate(&g_auto_features, reason, (int)sizeof(reason))) {
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, reason, 120);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, "Feature module validation failed", 140);
        }
        return;
    }

    if (strcmp(command_id, "file.new_project") == 0) {
        reset_project_workspace(app);
        ui_info(app, "New project workspace created");
        printf("[IDE] New project workspace created.\n");
    } else if (strcmp(command_id, "file.save_all") == 0) {
        if (app->sources.active_buffer) {
            source_workspace_save_active(&app->sources);
        }
        tilemap_save_csv(app->active_map, "autosave_tilemap.csv");
        project_io_save("autosave_project.db", app->project_db, app->active_map);
        editor_settings_save(&app->settings, "editor_settings.ini");
        ui_api_save_layout(&app->ui, "editor_ui_layout.ini");
        gbstudio_save_project(&app->gb_project, "gbstudio_project.db");
        asset_tools_generate_manifest("res", "autosave_manifest.json");
        ui_info(app, "Saved all project data");
        printf("[IDE] Save all complete.\n");
    } else if (strcmp(command_id, "file.import_asset") == 0) {
        char name[64];
        char path[120];
        int id;
        snprintf(name, sizeof(name), "imported_asset_%d", app->project_db ? (app->project_db->next_id) : 1);
        snprintf(path, sizeof(path), "res/imported/%s.dat", name);
        id = project_db_add_asset(app->project_db, DB_ASSET_OTHER, name, path, "imported");
        if (id > 0) {
            ui_info(app, "Asset entry created (placeholder import)");
            printf("[IDE] Imported asset placeholder id=%d path=%s\n", id, path);
        } else {
            ui_warn(app, "Asset import failed");
        }
    } else if (strcmp(command_id, "file.import_map_data") == 0) {
        if (tilemap_load_csv_inplace(app->active_map, "autosave_tilemap.csv")) {
            ui_info(app, "Map data imported from autosave_tilemap.csv");
            printf("[IDE] Imported map data from autosave_tilemap.csv\n");
        } else {
            ui_warn(app, "Map import failed (missing or invalid autosave_tilemap.csv)");
        }
    } else if (strcmp(command_id, "edit.undo") == 0) {
        tool_undo_last(app->active_map, &app->commands);
        ui_info(app, "Undo");
    } else if (strcmp(command_id, "edit.redo") == 0) {
        tool_redo_last(app->active_map, &app->commands);
        ui_info(app, "Redo");
    } else if (strcmp(command_id, "edit.duplicate_selection") == 0) {
        int x, y, tile;
        if (!app->active_map) {
            ui_warn(app, "No active map to duplicate from");
        } else {
            x = (app->frame_count / 3) % app->active_map->width;
            y = (app->frame_count / 5) % app->active_map->height;
            tile = tilemap_get(app->active_map, x, y);
            if (tilemap_set(app->active_map, (x + 1) % app->active_map->width, y, tile)) {
                ui_info(app, "Duplicated sample selection");
            } else {
                ui_warn(app, "Duplicate selection failed");
            }
        }
    } else if (strcmp(command_id, "edit.delete_selection") == 0) {
        int cx, cy;
        if (!app->active_map) {
            ui_warn(app, "No active map to delete from");
        } else {
            cx = (app->frame_count / 4) % app->active_map->width;
            cy = (app->frame_count / 6) % app->active_map->height;
            if (tilemap_fill_rect(app->active_map, cx - 1, cy - 1, cx + 1, cy + 1, 0)) {
                ui_info(app, "Deleted sample selection");
            } else {
                ui_warn(app, "Delete selection failed");
            }
        }
    } else if (strcmp(command_id, "editor.preferences.open") == 0) {
        char msg[256];
        snprintf(msg, sizeof(msg),
            "Autosave=%s every %ds | Theme=%s | Backend=%s",
            app->autosave_enabled ? "ON" : "OFF",
            app->settings.project.autosave_seconds,
            editor_theme_name(app->theme_id),
            editor_backend_name(app->backend));
        ui_api_open_dialog(&app->ui, "editor_prefs", "Editor Preferences", msg);
        ui_info(app, "Editor preferences opened");
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
    } else if (strcmp(command_id, "file.export.build_package") == 0) {
        BuildOutputBuffer out;
        asset_tools_generate_manifest("res", "autosave_manifest.json");
        build_run_project("build.bat", &out);
        ui_info(app, "Build package export complete");
        printf("[IDE] Build package export executed. lines=%d exit=%d\n", out.line_count, out.last_exit_code);
    } else if (strcmp(command_id, "tools.paint") == 0) {
        app->current_tool = TOOL_PAINT;
        ui_info(app, "Tool: Paint");
    } else if (strcmp(command_id, "tools.fill") == 0) {
        app->current_tool = TOOL_FILL;
        ui_info(app, "Tool: Fill");
    } else if (strcmp(command_id, "tools.erase") == 0) {
        app->current_tool = TOOL_ERASE;
        ui_info(app, "Tool: Erase");
    } else if (strcmp(command_id, "tools.object") == 0) {
        app->current_tool = TOOL_PLACE_OBJECT;
        ui_info(app, "Tool: Object");
    } else if (strcmp(command_id, "view.theme.classic") == 0) {
        apply_theme(app, THEME_CLASSIC_DARK);
    } else if (strcmp(command_id, "view.theme.blueprint") == 0) {
        apply_theme(app, THEME_BLUEPRINT_NOIR);
    } else if (strcmp(command_id, "view.theme.retro") == 0) {
        apply_theme(app, THEME_RETRO_AMBER);
    } else if (strcmp(command_id, "view.theme.contrast") == 0) {
        apply_theme(app, THEME_HIGH_CONTRAST);
    } else if (strcmp(command_id, "view.theme.vscode.dark_plus") == 0) {
        apply_theme(app, THEME_VSCODE_DARK_PLUS);
    } else if (strcmp(command_id, "view.theme.vscode.light_plus") == 0) {
        apply_theme(app, THEME_VSCODE_LIGHT_PLUS);
    } else if (strcmp(command_id, "view.theme.vscode.monokai") == 0) {
        apply_theme(app, THEME_VSCODE_MONOKAI);
    } else if (strcmp(command_id, "view.theme.vscode.solarized_dark") == 0) {
        apply_theme(app, THEME_VSCODE_SOLARIZED_DARK);
    } else if (strcmp(command_id, "view.theme.vscode.solarized_light") == 0) {
        apply_theme(app, THEME_VSCODE_SOLARIZED_LIGHT);
    } else if (strcmp(command_id, "view.theme.vscode.abyss") == 0) {
        apply_theme(app, THEME_VSCODE_ABYSS);
    } else if (strcmp(command_id, "view.theme.vscode.kimbie_dark") == 0) {
        apply_theme(app, THEME_VSCODE_KIMBIE_DARK);
    } else if (strcmp(command_id, "view.theme.vscode.red") == 0) {
        apply_theme(app, THEME_VSCODE_RED);
    } else if (strcmp(command_id, "view.theme.vscode.quiet_light") == 0) {
        apply_theme(app, THEME_VSCODE_QUIET_LIGHT);
    } else if (strcmp(command_id, "blueprint.new_graph") == 0) {
        ui_info(app, "New blueprint graph created");
        printf("[Blueprint] Created new graph context\n");
    } else if (strcmp(command_id, "blueprint.compile_graph") == 0) {
        BuildOutputBuffer out;
        build_run_project("build.bat", &out);
        ui_info(app, "Blueprint graph compiled (project build invoked)");
        printf("[Blueprint] Compile requested. lines=%d exit=%d\n", out.line_count, out.last_exit_code);
    } else if (strcmp(command_id, "blueprint.validate_graph") == 0) {
        EncounterBudget b;
        char reason[128];
        b.chapter = 2;
        b.recommended_party_level_min = 2;
        b.recommended_party_level_max = 5;
        b.enemy_cr_total = 10;
        game_tools_validate_encounter(&b, reason, sizeof(reason));
        ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, reason, 180);
        printf("[Blueprint] Validation: %s\n", reason);
    } else if (strcmp(command_id, "view.panel.world_hierarchy") == 0) {
        toggle_panel_visibility(app, "panel.hierarchy", "World Hierarchy");
    } else if (strcmp(command_id, "view.panel.asset_outliner") == 0) {
        toggle_panel_visibility(app, "panel.outliner", "Asset Outliner");
    } else if (strcmp(command_id, "view.panel.details") == 0) {
        toggle_panel_visibility(app, "panel.details", "Details");
    } else if (strcmp(command_id, "view.panel.output_log") == 0) {
        toggle_panel_visibility(app, "panel.console", "Output Log");
    } else if (strcmp(command_id, "view.panel.profiler") == 0) {
        toggle_panel_visibility(app, "panel.profiler", "Profiler");
    } else if (strcmp(command_id, "engine.validate.encounter") == 0) {
        EncounterBudget b;
        char reason[128];
        b.chapter = 3;
        b.recommended_party_level_min = 4;
        b.recommended_party_level_max = 6;
        b.enemy_cr_total = 18;
        game_tools_validate_encounter(&b, reason, sizeof(reason));
        printf("[IDE] Encounter validation: %s\n", reason);
    } else if (strcmp(command_id, "engine.rebuild.worldgen") == 0) {
        BuildOutputBuffer out;
        build_run_project("build.bat", &out);
        ui_info(app, "Build/worldgen run complete (see console log)");
        printf("[IDE] Build/worldgen executed. lines=%d exit=%d\n", out.line_count, out.last_exit_code);
    } else if (strcmp(command_id, "engine.runtime.tick") == 0) {
        engine_runtime_tick(&app->runtime);
        ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->runtime.last_event, 120);
        printf("[Runtime] %s\n", app->runtime.last_event);
    } else if (strcmp(command_id, "engine.runtime.spawn_enemy") == 0) {
        char nm[32];
        snprintf(nm, sizeof(nm), "Enemy%d", app->runtime.turn + app->runtime.entity_count + 1);
        if (engine_runtime_spawn_enemy(&app->runtime, nm, 10 + (app->runtime.turn % 7), 18, 10)) {
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->runtime.last_event, 140);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, "Runtime enemy spawn failed", 160);
        }
        printf("[Runtime] %s\n", app->runtime.last_event);
    } else if (strcmp(command_id, "engine.runtime.attack") == 0) {
        engine_runtime_player_attack(&app->runtime);
        ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->runtime.last_event, 120);
        printf("[Runtime] %s\n", app->runtime.last_event);
    } else if (strcmp(command_id, "engine.quest.next") == 0) {
        engine_runtime_quest_next(&app->runtime);
        ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->runtime.last_event, 120);
    } else if (strcmp(command_id, "engine.quest.progress") == 0) {
        engine_runtime_quest_progress(&app->runtime, 1);
        ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->runtime.last_event, 120);
    } else if (strcmp(command_id, "engine.quest.complete") == 0) {
        if (engine_runtime_quest_complete_active(&app->runtime)) {
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->runtime.last_event, 140);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, "Quest not ready to complete", 140);
        }
        printf("[Runtime] %s\n", app->runtime.last_event);
    } else if (strcmp(command_id, "gb.scene.new") == 0) {
        char name[48];
        int scene_id;
        snprintf(name, sizeof(name), "Scene %d", app->gb_project.next_scene_id);
        scene_id = gbstudio_add_scene(&app->gb_project, name, 32, 24, 1);
        ui_info(app, "GB scene created");
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
        if (actor_id > 0) ui_info(app, "Actor added to active scene");
        else ui_warn(app, "Failed to add actor");
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
        if (actor_id > 0) ui_info(app, "Hostile actor added");
        else ui_warn(app, "Failed to add hostile actor");
        printf("[GB Studio] Added hostile actor id=%d in scene=%d\n", actor_id, app->gb_project.active_scene_id);
    } else if (strcmp(command_id, "gb.trigger.add") == 0) {
        int event_id = gbstudio_add_script_event(&app->gb_project, GB_SCRIPT_HEAL_PLAYER, 2, 0, 0);
        int trigger_id = gbstudio_add_trigger(&app->gb_project, app->gb_project.active_scene_id, "Heal Zone", 14, 11, 2, 2, event_id);
        if (trigger_id > 0) ui_info(app, "Trigger added and linked to script");
        else ui_warn(app, "Failed to add trigger");
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
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, result, 180);
            printf("[GB Studio] Script result: %s\n", result);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, result, 200);
            printf("[GB Studio] Script failed: %s\n", result);
        }
    } else if (strcmp(command_id, "gb.validate.scene") == 0) {
        char reason[128];
        if (gbstudio_validate_scene(&app->gb_project, app->gb_project.active_scene_id, reason, sizeof(reason))) {
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, reason, 190);
            printf("[GB Studio] %s\n", reason);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, reason, 220);
            printf("[GB Studio] Validation failed: %s\n", reason);
        }
    } else if (strcmp(command_id, "gb.validate.all") == 0) {
        char reason[128];
        if (gbstudio_validate_all_scenes(&app->gb_project, reason, sizeof(reason))) {
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, reason, 190);
            printf("[GB Studio] %s\n", reason);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, reason, 220);
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
        if (app->gb_play_mode) {
            gbstudio_start_loading_screen(app);
            ui_info(app, "Loading GB Studio game...");
        } else {
            gbstudio_stop_loading_screen(app);
        }
        printf("[GB Studio] Play mode: %s\n", app->gb_play_mode ? "ON" : "OFF");
    } else if (strcmp(command_id, "gb.play.stop") == 0) {
        if (app->gb_play_mode) {
            app->gb_play_mode = false;
            gbstudio_stop_loading_screen(app);
            ui_api_set_widget_enabled(&app->ui, "widget.toolbar.play", true);
            ui_api_set_widget_enabled(&app->ui, "widget.toolbar.stop", false);
            ui_info(app, "Play mode stopped");
        } else {
            ui_warn(app, "Play mode already stopped");
        }
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
    } else if (strcmp(command_id, "gb.project.export_rom_config") == 0) {
        FILE *f = fopen("gbstudio_rom_config.ini", "w");
        if (f) {
            fprintf(f, "[project]\n");
            fprintf(f, "active_scene_id=%d\n", app->gb_project.active_scene_id);
            fprintf(f, "scene_count=%d\n", app->gb_project.scene_count);
            fprintf(f, "actor_count=%d\n", app->gb_project.actor_count);
            fprintf(f, "trigger_count=%d\n", app->gb_project.trigger_count);
            fprintf(f, "script_event_count=%d\n", app->gb_project.script_event_count);
            fclose(f);
            ui_info(app, "Exported gbstudio_rom_config.ini");
            printf("[GB Studio] Exported ROM config: gbstudio_rom_config.ini\n");
        } else {
            ui_warn(app, "Failed to export ROM config");
        }
    } else if (strcmp(command_id, "editor.command_palette.toggle") == 0) {
        app->command_palette_open = !app->command_palette_open;
        if (app->command_palette_index < 0) app->command_palette_index = 0;
        ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->command_palette_open ? "Command palette opened" : "Command palette closed", 140);
    } else if (strcmp(command_id, "editor.command_palette.next") == 0) {
        int count = palette_visible_count(app);
        if (count > 0) {
            app->command_palette_index = (app->command_palette_index + 1) % count;
        }
    } else if (strcmp(command_id, "editor.command_palette.prev") == 0) {
        int count = palette_visible_count(app);
        if (count > 0) {
            app->command_palette_index--;
            if (app->command_palette_index < 0) app->command_palette_index = count - 1;
        }
    } else if (strcmp(command_id, "editor.command_palette.run") == 0) {
        run_selected_palette_command(app);
    } else if (strcmp(command_id, "window.layout.load_unreal_gb") == 0) {
        if (ui_api_load_layout(&app->ui, "editor_ui_layout.ini")) {
            ui_info(app, "Layout loaded: Unreal GB preset");
        } else {
            set_all_panels_visible(app, true);
            ui_warn(app, "Layout preset missing, restored visible panels");
        }
    } else if (strcmp(command_id, "window.layout.save_current") == 0) {
        if (ui_api_save_layout(&app->ui, "editor_ui_layout.ini")) {
            ui_info(app, "Current layout saved");
        } else {
            ui_warn(app, "Layout save failed");
        }
    } else if (strcmp(command_id, "window.layout.reset") == 0) {
        set_all_panels_visible(app, true);
        ui_api_close_dialog(&app->ui);
        ui_info(app, "Layout reset to visible defaults");
    } else if (strcmp(command_id, "help.quick_start") == 0) {
        ui_api_open_dialog(&app->ui, "quick_start", "Quick Start",
            "New Project -> Add Scene/Actor/Trigger -> Play -> Save All");
        ui_info(app, "Quick Start opened");
    } else if (strcmp(command_id, "help.controls") == 0) {
        show_help_screen();
        ui_info(app, "Controls printed to console");
    } else if (strcmp(command_id, "help.about") == 0) {
        show_about_screen();
        ui_info(app, "About screen printed to console");
    } else if (strcmp(command_id, "source.insert.stub") == 0) {
        if (!app->sources.active_buffer) {
            source_workspace_open_active(&app->sources);
        }
        if (app->sources.active_buffer) {
            editor_file_insert(app->sources.active_buffer, 0, "/* TODO: implement feature */\n");
            app->sources.active_buffer->cursor_line = 0;
            snprintf(app->sources.last_status, sizeof(app->sources.last_status), "inserted stub line in %s",
                source_workspace_active(&app->sources) ? source_workspace_active(&app->sources)->name : "source");
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->sources.last_status, 180);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, "No active source file", 180);
        }
    } else if (strcmp(command_id, "source.delete.line") == 0) {
        if (app->sources.active_buffer && app->sources.active_buffer->line_count > 0) {
            int line = app->sources.active_buffer->cursor_line;
            if (line < 0) line = 0;
            if (line >= app->sources.active_buffer->line_count) line = app->sources.active_buffer->line_count - 1;
            editor_file_delete(app->sources.active_buffer, line);
            if (line >= app->sources.active_buffer->line_count) line = app->sources.active_buffer->line_count - 1;
            if (line < 0) line = 0;
            app->sources.active_buffer->cursor_line = line;
            snprintf(app->sources.last_status, sizeof(app->sources.last_status), "deleted line %d", line + 1);
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->sources.last_status, 180);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, "No source line to delete", 180);
        }
    } else if (strcmp(command_id, "source.search.includes") == 0) {
        int next;
        strncpy(app->source_search_term, "#include", sizeof(app->source_search_term) - 1);
        app->source_search_term[sizeof(app->source_search_term) - 1] = '\0';
        if (!app->sources.active_buffer) {
            source_workspace_open_active(&app->sources);
        }
        next = source_find_next_line(&app->sources, app->source_search_term,
            app->sources.active_buffer ? (app->sources.active_buffer->cursor_line + 1) : 0);
        if (next < 0) {
            next = source_find_next_line(&app->sources, app->source_search_term, 0);
        }
        if (next >= 0 && app->sources.active_buffer) {
            app->sources.active_buffer->cursor_line = next;
            app->source_search_line = next;
            snprintf(app->sources.last_status, sizeof(app->sources.last_status), "found include at line %d", next + 1);
            ui_api_push_notification(&app->ui, UI_NOTIFY_INFO, app->sources.last_status, 180);
        } else {
            ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, "No include line found", 180);
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
    } else {
        const EditorCommandDef *def = command_registry_find(&app->command_registry, command_id);
        char msg[180];
        if (def) {
            snprintf(msg, sizeof(msg), "Command not implemented yet: %s", command_id);
        } else {
            snprintf(msg, sizeof(msg), "Unknown command: %s", command_id);
        }
        ui_warn(app, msg);
        printf("[IDE] %s\n", msg);
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

    if (app->command_palette_open) {
        int len = (int)strlen(app->command_palette_filter);
        if (ch == 27) {
            execute_command(app, "editor.command_palette.toggle");
        } else if (ch == 13) {
            execute_command(app, "editor.command_palette.run");
        } else if (ch == KEY_UP) {
            execute_command(app, "editor.command_palette.prev");
        } else if (ch == KEY_DOWN) {
            execute_command(app, "editor.command_palette.next");
        } else if (ch == 8) {
            if (len > 0) {
                app->command_palette_filter[len - 1] = '\0';
                app->command_palette_index = 0;
            }
        } else if (ch >= 32 && ch <= 126 && len < (int)sizeof(app->command_palette_filter) - 1) {
            app->command_palette_filter[len] = (char)ch;
            app->command_palette_filter[len + 1] = '\0';
            app->command_palette_index = 0;
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
        case 'i': case 'I': execute_command(app, "engine.runtime.tick"); break;
        case 'f': case 'F': execute_command(app, "engine.runtime.attack"); break;
        case ']': execute_command(app, "engine.quest.progress"); break;
        case '[': execute_command(app, "engine.quest.next"); break;
        case KEY_F5:
            build_run_project("build.bat", &build_output);
            show_build_output = 1;
            break;
        case KEY_F6:
            build_run_project("ide_editor_x64.exe", &build_output);
            show_build_output = 1;
            break;
        case KEY_F3:
            execute_command(app, "source.search.includes");
            break;
        case KEY_F4:
            execute_command(app, "source.save");
            break;
        case KEY_F7:
            execute_command(app, "source.scan");
            break;
        case KEY_F8:
            execute_command(app, "source.compile.active");
            break;
        case KEY_F9:
            execute_command(app, "editor.command_palette.toggle");
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
    app->command_palette_open = false;
    app->command_palette_index = 0;
    app->command_palette_filter[0] = '\0';
    app->source_search_line = -1;
    app->source_search_term[0] = '\0';
    app->last_command[0] = '\0';
    app->gb_play_mode = false;
    app->gb_loading_screen_active = false;
    app->gb_loading_progress_percent = 0;
    app->gb_loading_frames_left = 0;
    app->gb_loading_subtitle[0] = '\0';
    source_workspace_init(&app->sources, "..\\src");
    engine_runtime_init(&app->runtime);
    engine_runtime_seed_defaults(&app->runtime);
    auto_feature_registry_init(&g_auto_features);

    register_commands(app);
    build_menus(app);
    menu_interaction_build(&app->interactive_menu, app->top_menu);
    build_ui_layout(app);
    build_sidebar(app);
    register_tools(app);
    editor_seed_default_content(app);
    gbstudio_seed_default_content(app);
    {
        char reason[128];
        if (!auto_feature_registry_validate(&g_auto_features, reason, (int)sizeof(reason))) {
            ui_api_push_notification(&app->ui, UI_NOTIFY_WARN, reason, 180);
        }
    }
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
    printf("      F3 include-search, F4 source-save, F7 source-scan, F8 source-compile, F9 command palette,\n");
    printf("      I runtime-tick, F runtime-attack, [ quest-next, ] quest-progress,\n");
    printf("      G/H/J/K/V GB authoring, B validate-all, N/X scene nav, L play mode, Y save, Z load, Q/Esc quit.\n");
    ui_api_render_unreal_layout(&app->ui);

    while (editor_backend_is_running(app->backend) && app->frame_count < max_frames) {
        char overlay[16384];
        EditorUIAction actions[EDITOR_UI_MAX_ACTIONS];
        int action_count = 0;
        char clicked_cmd[128];
        build_overlay_text(app, overlay, (int)sizeof(overlay));
        build_ui_actions(app, actions, &action_count, EDITOR_UI_MAX_ACTIONS);
        editor_backend_set_overlay(app->backend, overlay);
        editor_backend_set_loading_screen(
            app->backend,
            app->gb_loading_screen_active,
            "GB Studio - Loading Game",
            app->gb_loading_subtitle,
            app->gb_loading_progress_percent
        );
        editor_backend_set_ui_actions(app->backend, actions, action_count);

        editor_backend_poll(app->backend);
        editor_backend_render(app->backend);
        while (editor_backend_pop_ui_action(app->backend, clicked_cmd, (int)sizeof(clicked_cmd))) {
            handle_ui_action(app, clicked_cmd);
        }
        if (app->gb_play_mode && app->gb_loading_screen_active) {
            gbstudio_update_loading_screen(app);
        } else if (app->gb_play_mode) {
            gbstudio_tick_game_logic(&app->gb_project);
            if ((app->frame_count % 15) == 0) {
                engine_runtime_tick(&app->runtime);
            }
        }
        auto_feature_registry_tick(&g_auto_features);
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
