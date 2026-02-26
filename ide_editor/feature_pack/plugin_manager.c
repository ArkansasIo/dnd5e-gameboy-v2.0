#include ".h"
#include <stdio.h>
#include <string.h>

void _init(_config *cfg) {
    if (!cfg) return;
    cfg->enabled = true;
    cfg->revision = 1;
}

bool _validate(const _config *cfg, char *reason, int reason_len) {
    if (reason && reason_len > 0) {
        snprintf(reason, (size_t)reason_len, ": OK");
    }
    if (!cfg) return false;
    return cfg->enabled;
}

void _tick(_config *cfg) {
    if (!cfg || !cfg->enabled) return;
    cfg->revision++;
}