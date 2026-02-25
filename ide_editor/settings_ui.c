#include "settings_ui.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

SettingsUI *settings_ui_create(void) {
    SettingsUI *ui = (SettingsUI *)malloc(sizeof(SettingsUI));
    ui->item_count = 2;
    strncpy(ui->items[0].key, "Theme", 63);
    strncpy(ui->items[0].value, "Dark", 127);
    strncpy(ui->items[1].key, "Font Size", 63);
    strncpy(ui->items[1].value, "14", 127);
    return ui;
}

void settings_ui_destroy(SettingsUI *ui) {
    free(ui);
}

void settings_ui_draw(const SettingsUI *ui) {
    printf("--- Settings ---\n");
    for (int i = 0; i < ui->item_count; ++i) {
        printf("%s: %s\n", ui->items[i].key, ui->items[i].value);
    }
    printf("(Use arrow keys to navigate, Enter to edit)\n");
}
