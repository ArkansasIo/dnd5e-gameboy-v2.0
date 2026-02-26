#include "splash.h"
#include <stdio.h>
#include <windows.h>

void show_splash_screen(void) {
    system("cls");
    for (int i = 0; i < 7; ++i) {
        printf("%s\n", IDE_EDITOR_LOGO[i]);
    }
    printf("\n      Welcome to EditorIDE!\n");
    printf("      Modular C/C++ Game & App Editor\n");
    printf("      Build: %s\n", __DATE__);
    Sleep(2000); // Pause for 2 seconds
}
