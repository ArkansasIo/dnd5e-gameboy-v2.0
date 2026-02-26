#ifndef IDE_EDITOR_H
#define IDE_EDITOR_H

#include <stdbool.h>
#include <stdint.h>
#include "project_db.h"
#include "map_editor.h"
#include "command_stack.h"
#include "tool_modes.h"
#include "editor_settings.h"
#include "editor_theme.h"
#include "editor_tools.h"
#include "ui_api.h"
#include "command_registry.h"
#include "editor_backend.h"
#include "gb_studio_tools.h"
#include "menu_interaction.h"
#include "source_tools.h"
#include "engine_runtime.h"

// Forward declarations
struct EditorWindow;
struct EditorMenu;
struct EditorSidebar;
struct EditorWidget;
struct EditorBackend;

// Main IDE Editor App
typedef struct {
    struct EditorWindow *main_window;
    struct EditorBackend *backend;
    struct EditorMenu *top_menu;
    struct EditorSidebar *sidebar;
    struct EditorWidget *widgets[8];
    int widget_count;
    ProjectDB *project_db;
    TileMap *active_map;
    CommandStack commands;
    EditorToolMode current_tool;
    int selected_tile_id;
    EditorSettings settings;
    EditorThemeId theme_id;
    UIContext ui;
    EditorToolRegistry tool_registry;
    EditorCommandRegistry command_registry;
    InteractiveMenuState interactive_menu;
    bool mouse_menu_open;
    int mouse_menu_top;
    bool command_palette_open;
    int command_palette_index;
    char command_palette_filter[32];
    SourceWorkspace sources;
    int source_search_line;
    char source_search_term[32];
    char last_command[128];
    EngineRuntime runtime;
    GBStudioProject gb_project;
    bool gb_play_mode;
    bool gb_loading_screen_active;
    int gb_loading_progress_percent;
    int gb_loading_frames_left;
    char gb_loading_subtitle[128];
    int frame_count;
    bool autosave_enabled;
} IDEEditorApp;

// API
IDEEditorApp *ide_editor_create(void);
IDEEditorApp *ide_editor_create_with_backend(EditorBackendMode mode);
void ide_editor_run(IDEEditorApp *app);
void ide_editor_destroy(IDEEditorApp *app);

#endif // IDE_EDITOR_H
