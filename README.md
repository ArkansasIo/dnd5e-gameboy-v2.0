# Labyrinth of the Dragon (GBC)
An 8-bit Adventure RPG with D&D Monsters!

## Documentation

Project design and engineering docs are in [docs/README.md](docs/README.md).

## How to Build the ROM

### Depedencies
* [GBDK-2020](https://github.com/gbdk-2020/gbdk-2020) - The Game Boy Development
  kit. Includes the libraries and binaries for C development on the Game Boy.
* [GNU Make](https://gnuwin32.sourceforge.net/packages/make.htm) - Build system
  tool (installation should only be required on Windows).
* [NodeJS](https://nodejs.org) - Used to run custom tools I made in the course
  of developing the game.

### Use Make to Build the ROM
Update the `MakeFile` or define a shell variable named `GBDK_HOME` pointing to
the directory where you installed GBDK.

To build the ROM run the following commands:

* `npm install`
* `make assets`
* `make`

## GBDK-2020 for 32-bit and 64-bit Windows

To build this project on Windows, you need GBDK-2020. Download the version that matches your OS:

- **64-bit Windows:** [gbdk-win64.zip](https://github.com/gbdk-2020/gbdk-2020/releases/latest/download/gbdk-win64.zip)
- **32-bit Windows:** [gbdk-win32.zip](https://github.com/gbdk-2020/gbdk-2020/releases/latest/download/gbdk-win32.zip)

Extract the archive (e.g., to `D:/gbdk`).

Add `D:/gbdk/bin` to your system PATH for command-line tools (optional).

Update your include path in VS Code or your build system to point to `D:/gbdk/include`.

The engine and all config files are compatible with both 32-bit and 64-bit systems.

## Save and Load System

This game supports saving and loading progress using the Game Boy's battery-backed SRAM. There are 3 save slots. The save system stores basic player data (level, HP, XP, etc.) and can be expanded as needed.

### How it works
- **Saving:** When you save, the current player state is written to SRAM in the selected slot.
- **Loading:** When you load, the player state is restored from SRAM.
- **Erasing:** You can erase a save slot to clear its data.

### Technical details
- Save data is defined in `src/saveload.h` and implemented in `src/saveload.c`.
- Integration with the main menu is in `src/main_menu.c`.
- SRAM is accessed using GBDK's `ENABLE_RAM`/`DISABLE_RAM` macros.

### Extending Save Data
To add more fields to the save file, update the `SaveData` struct in `src/saveload.h` and adjust the save/load logic as needed.

## IDE Editor: Modular UI Usage Example

Below is a C code snippet showing how to dynamically add a new docked panel and a button widget to the Unreal 5-style UI at runtime using the UI API:

```c
#include "ui_api.h"

void add_custom_panel_and_button(UIContext *ctx) {
    // Add a new docked panel on the right
    ui_api_add_panel(ctx, "panel.custom", "Custom Tools", UI_DOCK_RIGHT);
    // Add a button widget to the new panel
    ui_api_add_widget(ctx, "widget.custom.action", "panel.custom", "Run Custom Action", UI_WIDGET_BUTTON);
}
```

- Call this function after initializing your UI context (e.g., after `ui_api_init`).
- The new panel and button will appear in the running IDE editor UI.
- You can use similar calls to add toggles, sliders, input fields, or more panels/widgets.

### Example: Adding a Toggle and Slider Widget

You can add other widget types such as toggles and sliders to any panel:

```c
// Add a toggle to the custom panel
ui_api_add_widget(ctx, "widget.custom.toggle", "panel.custom", "Enable Feature", UI_WIDGET_TOGGLE);
// Add a slider to the custom panel
ui_api_add_widget(ctx, "widget.custom.slider", "panel.custom", "Adjust Value", UI_WIDGET_SLIDER);
```

- Use `UI_WIDGET_TOGGLE` for checkboxes/switches.
- Use `UI_WIDGET_SLIDER` for numeric sliders.
- Use `UI_WIDGET_INPUT` for text input fields.

All widgets can be shown/hidden, enabled/disabled, and updated at runtime using the UI API.

### Example: Handling Widget Events in C

To respond to button or toggle events, add logic in your main event loop or a dedicated handler. For example:

```c
#include "ui_api.h"

void handle_ui_events(UIContext *ctx) {
    UIWidget *btn = ui_api_find_widget(ctx, "widget.custom.action");
    if (btn && btn->kind == UI_WIDGET_BUTTON && btn->int_value == 1) {
        // Button was pressed
        // ...perform your custom action...
        btn->int_value = 0; // Reset after handling
    }
    UIWidget *toggle = ui_api_find_widget(ctx, "widget.custom.toggle");
    if (toggle && toggle->kind == UI_WIDGET_TOGGLE) {
        if (toggle->int_value) {
            // Toggle is ON
        } else {
            // Toggle is OFF
        }
    }
}
```

- Call `handle_ui_events(&app->ui);` in your main loop after polling input.
- For buttons, set `int_value = 1` when pressed (reset after handling).
- For toggles, `int_value` is 1 (on) or 0 (off).

This pattern lets you connect UI widgets to your editor logic in a modular way.

### Example: Interactive Menus and Submenus

The UI API supports interactive top menus and nested submenus. You can define menu items and handle their selection in your event loop.

```c
#include "ui_api.h"

void handle_menu_events(UIContext *ctx) {
    // Example: Check if a menu item was selected
    const UIMenuItem *item = ui_api_find_menu_item(ctx, "File/New Project");
    if (item && item->enabled && item->checked) {
        // The "New Project" menu item was activated
        // ...perform new project logic...
        // Reset checked state after handling
        ((UIMenuItem *)item)->checked = false;
    }
}
```

- Add menu items with `ui_api_add_menu_item(ctx, "File/New Project", "file.new_project");`
- Use `/` in the path for submenus (e.g., `File/Open`, `Edit/Undo`).
- Set `checked = true` when a menu item is selected (from your input handler or UI logic).
- Handle menu actions in your main loop or a dedicated handler.

This approach allows you to build interactive, hierarchical menus similar to Unreal Engine 5's top bar and submenus.

For more advanced usage, see the `ui_api.h` and `ui_api.c` files for all available API functions.

---

## Enabling a Graphical UI (SDL2/ImGui)

Your IDE editor supports multiple backends. To enable a graphical UI:

### 1. Build with SDL2 or ImGui Support
- Make sure SDL2 and/or ImGui are installed and available to your compiler.
- Link against SDL2/ImGui in your build system (update `build.bat` or Makefile if needed).

### 2. Launch with Graphical Backend
- Run the editor with the backend flag:
  - For SDL2: `ide_editor.exe --backend sdl2`
  - For ImGui: `ide_editor.exe --backend imgui`

### 3. Example: Minimal ImGui Main Loop

```c
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>
#include <SDL.h>

void run_imgui_ui() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("IDE Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) running = false;
        }
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        // Draw your panels, menus, widgets here
        ImGui::Begin("Hello, Unreal 5 Style!");
        ImGui::Text("This is a graphical UI window.");
        ImGui::End();
        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
```

### 4. Adding Graphical Widgets
- Use ImGui functions (e.g., `ImGui::Button`, `ImGui::MenuBar`, `ImGui::Begin`, `ImGui::End`) to create windows, menus, and controls.
- Map your modular UI API to ImGui calls for a seamless experience.

### 5. Next Steps
- Update your build scripts to include SDL2/ImGui.
- Add ImGui/SDL2 backend initialization in your code (see `editor_backend.c`).
- Use the `--backend` flag to switch between console and graphical modes.

---

This approach gives you a full graphical Unreal 5-style UI with mouse, window, and advanced widget support. Let me know if you want a full integration example or help updating your build scripts!
