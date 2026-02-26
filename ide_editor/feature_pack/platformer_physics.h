#ifndef PLATFORMER_PHYSICS_H
#define PLATFORMER_PHYSICS_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bool enabled;
    int revision;
} _config;

void _init(_config *cfg);
bool _validate(const _config *cfg, char *reason, int reason_len);
void _tick(_config *cfg);

#ifdef __cplusplus
}
#endif

#endif