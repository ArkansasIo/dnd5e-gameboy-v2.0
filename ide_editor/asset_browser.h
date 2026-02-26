#ifndef ASSET_BROWSER_H
#define ASSET_BROWSER_H

#define ASSET_BROWSER_MAX_FILES 512

// Asset browser entry
typedef struct {
    char path[256];
    int is_selected;
} AssetBrowserEntry;

typedef struct {
    AssetBrowserEntry files[ASSET_BROWSER_MAX_FILES];
    int file_count;
    int selected_index;
} AssetBrowser;

AssetBrowser *asset_browser_create(void);
void asset_browser_destroy(AssetBrowser *browser);
void asset_browser_scan(AssetBrowser *browser, const char *directory);
void asset_browser_draw(const AssetBrowser *browser);

#endif // ASSET_BROWSER_H
