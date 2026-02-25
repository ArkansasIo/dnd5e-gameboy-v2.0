#ifndef ASSET_TOOLS_H
#define ASSET_TOOLS_H

#include <stdbool.h>

bool asset_tools_copy_file(const char *src, const char *dst);
bool asset_tools_generate_manifest(const char *folder, const char *out_file);

#endif
