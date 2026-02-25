#include "asset_browser_ui.h"
#include <stdlib.h>
#include <stdio.h>

AssetBrowserUI *asset_browser_ui_create(void) {
    AssetBrowserUI *ui = (AssetBrowserUI *)malloc(sizeof(AssetBrowserUI));
    ui->browser = asset_browser_create();
    ui->is_active = 1;
    return ui;
}

void asset_browser_ui_destroy(AssetBrowserUI *ui) {
    if (ui) {
        asset_browser_destroy(ui->browser);
        free(ui);
    }
}

void asset_browser_ui_draw(const AssetBrowserUI *ui) {
    if (!ui || !ui->browser) return;
    asset_browser_draw(ui->browser);
}

void asset_browser_ui_handle_input(AssetBrowserUI *ui, int key) {
    (void)ui;
    (void)key;
    // Placeholder for input handling
}
