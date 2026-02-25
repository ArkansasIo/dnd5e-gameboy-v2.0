#include "asset_browser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

AssetBrowser *asset_browser_create(void) {
    AssetBrowser *browser = (AssetBrowser *)malloc(sizeof(AssetBrowser));
    browser->file_count = 0;
    browser->selected_index = -1;
    return browser;
}

void asset_browser_destroy(AssetBrowser *browser) {
    free(browser);
}

void asset_browser_scan(AssetBrowser *browser, const char *directory) {
    (void)directory;
    // Placeholder: In a real app, scan directory for files
    browser->file_count = 2;
    strncpy(browser->files[0].path, "assets/sprite.png", 255);
    browser->files[0].is_selected = 0;
    strncpy(browser->files[1].path, "assets/music.mod", 255);
    browser->files[1].is_selected = 0;
}

void asset_browser_draw(const AssetBrowser *browser) {
    printf("--- Asset Browser ---\n");
    for (int i = 0; i < browser->file_count; ++i) {
        printf("%s%s\n", browser->files[i].is_selected ? "> " : "  ", browser->files[i].path);
    }
}
