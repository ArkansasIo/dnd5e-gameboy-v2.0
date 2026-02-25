#ifndef ASSET_BROWSER_UI_H
#define ASSET_BROWSER_UI_H

#include "asset_browser.h"

// Asset browser UI wrapper
typedef struct {
    AssetBrowser *browser;
    int is_active;
} AssetBrowserUI;

AssetBrowserUI *asset_browser_ui_create(void);
void asset_browser_ui_destroy(AssetBrowserUI *ui);
void asset_browser_ui_draw(const AssetBrowserUI *ui);
void asset_browser_ui_handle_input(AssetBrowserUI *ui, int key);

#endif // ASSET_BROWSER_UI_H
