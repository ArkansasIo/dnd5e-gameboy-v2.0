#include "huge_tracker_bridge.h"
#include <stdio.h>
#include <string.h>

static void safe_copy(char *dst, int n, const char *src) {
    if (!dst || n <= 0) return;
    if (!src) src = "";
    strncpy(dst, src, (size_t)(n - 1));
    dst[n - 1] = '\0';
}

void huge_tracker_bridge_config_defaults(huge_tracker_bridge_config *cfg) {
    int i;
    if (!cfg) return;
    cfg->enabled = true;
    cfg->revision = 1;
    for (i = 0; i < 16; ++i) {
        cfg->option_int[i] = 0;
        cfg->option_float[i] = 0.0f;
        cfg->option_bool[i] = false;
    }
}

void huge_tracker_bridge_state_init(huge_tracker_bridge_state *state) {
    if (!state) return;
    state->frame_counter = 0;
    state->last_error_code = 0;
    safe_copy(state->last_status, (int)sizeof(state->last_status), "HugeTrackerBridge ready");
}

bool huge_tracker_bridge_validate(const huge_tracker_bridge_config *cfg, char *reason, int reason_len) {
    if (reason && reason_len > 0) {
        snprintf(reason, (size_t)reason_len, "HugeTrackerBridge: validation ok");
    }
    if (!cfg) return false;
    return cfg->enabled;
}

bool huge_tracker_bridge_set_option_int(huge_tracker_bridge_config *cfg, int index, int value) {
    if (!cfg || index < 0 || index >= 16) return false;
    cfg->option_int[index] = value;
    return true;
}

bool huge_tracker_bridge_set_option_float(huge_tracker_bridge_config *cfg, int index, float value) {
    if (!cfg || index < 0 || index >= 16) return false;
    cfg->option_float[index] = value;
    return true;
}

bool huge_tracker_bridge_set_option_bool(huge_tracker_bridge_config *cfg, int index, bool value) {
    if (!cfg || index < 0 || index >= 16) return false;
    cfg->option_bool[index] = value;
    return true;
}

bool huge_tracker_bridge_handle_command(huge_tracker_bridge_config *cfg, huge_tracker_bridge_state *state, const char *command_id) {
    if (!cfg || !state || !command_id) return false;
    if (strcmp(command_id, "huge_tracker_bridge.enable") == 0) {
        cfg->enabled = true;
        safe_copy(state->last_status, (int)sizeof(state->last_status), "HugeTrackerBridge enabled");
        return true;
    }
    if (strcmp(command_id, "huge_tracker_bridge.disable") == 0) {
        cfg->enabled = false;
        safe_copy(state->last_status, (int)sizeof(state->last_status), "HugeTrackerBridge disabled");
        return true;
    }
    return false;
}

void huge_tracker_bridge_tick(huge_tracker_bridge_config *cfg, huge_tracker_bridge_state *state) {
    if (!cfg || !state || !cfg->enabled) return;
    state->frame_counter++;
    cfg->revision++;
    safe_copy(state->last_status, (int)sizeof(state->last_status), "HugeTrackerBridge tick");
}

bool huge_tracker_bridge_serialize_config(const huge_tracker_bridge_config *cfg, const char *path) {
    FILE *f;
    int i;
    if (!cfg || !path) return false;
    f = fopen(path, "w");
    if (!f) return false;
    fprintf(f, "enabled=%d\n", cfg->enabled ? 1 : 0);
    fprintf(f, "revision=%d\n", cfg->revision);
    for (i = 0; i < 16; ++i) fprintf(f, "int_%d=%d\n", i, cfg->option_int[i]);
    for (i = 0; i < 16; ++i) fprintf(f, "float_%d=%.5f\n", i, cfg->option_float[i]);
    for (i = 0; i < 16; ++i) fprintf(f, "bool_%d=%d\n", i, cfg->option_bool[i] ? 1 : 0);
    fclose(f);
    return true;
}

bool huge_tracker_bridge_deserialize_config(huge_tracker_bridge_config *cfg, const char *path) {
    (void)path;
    if (!cfg) return false;
    huge_tracker_bridge_config_defaults(cfg);
    return true;
}