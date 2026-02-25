#include "help_about.h"
#include <stdio.h>

void show_help_screen(void) {
    printf("--- Help ---\n");
    printf("F1: Help\nF2: About\nF5: Build\nF6: Run\nCtrl+O: Open File\nCtrl+S: Save File\nCtrl+Q: Quit\n");
}

void show_about_screen(void) {
    printf("--- About EditorIDE ---\n");
    printf("EditorIDE v1.0\nA modular C/C++ IDE and game editor.\n");
    printf("Features:\n");
    printf("- Project/file management\n- Code/text editing\n- Blueprint scripting\n- Asset browser\n- Build/run integration\n- Settings/preferences\n- Help/about\n");
    printf("Copyright (c) 2026\n");
}
