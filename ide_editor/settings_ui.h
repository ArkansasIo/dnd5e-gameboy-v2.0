#ifndef SETTINGS_UI_H
#define SETTINGS_UI_H

#define SETTINGS_MAX_ITEMS 32

typedef struct {
    char key[64];
    char value[128];
} SettingsItem;

typedef struct {
    SettingsItem items[SETTINGS_MAX_ITEMS];
    int item_count;
} SettingsUI;

SettingsUI *settings_ui_create(void);
void settings_ui_destroy(SettingsUI *ui);
void settings_ui_draw(const SettingsUI *ui);

#endif // SETTINGS_UI_H
