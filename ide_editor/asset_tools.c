#include "asset_tools.h"
#include <stdio.h>
#include <string.h>

bool asset_tools_copy_file(const char *src, const char *dst) {
    FILE *in;
    FILE *out;
    char buf[4096];
    size_t n;
    if (!src || !dst) return false;
    in = fopen(src, "rb");
    if (!in) return false;
    out = fopen(dst, "wb");
    if (!out) {
        fclose(in);
        return false;
    }
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        if (fwrite(buf, 1, n, out) != n) {
            fclose(in);
            fclose(out);
            return false;
        }
    }
    fclose(in);
    fclose(out);
    return true;
}

bool asset_tools_generate_manifest(const char *folder, const char *out_file) {
    FILE *f;
    if (!folder || !out_file) return false;
    f = fopen(out_file, "w");
    if (!f) return false;
    fprintf(f, "{\n");
    fprintf(f, "  \"manifestVersion\": 1,\n");
    fprintf(f, "  \"root\": \"%s\",\n", folder);
    fprintf(f, "  \"note\": \"Populate with scanned assets via future dir walker.\"\n");
    fprintf(f, "}\n");
    fclose(f);
    return true;
}
