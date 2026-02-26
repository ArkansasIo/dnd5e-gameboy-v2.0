#ifndef COMMAND_REGISTRY_H
#define COMMAND_REGISTRY_H

#include <stdbool.h>

#define COMMAND_MAX 256

typedef struct {
    char id[64];
    char description[128];
} EditorCommandDef;

typedef struct {
    EditorCommandDef defs[COMMAND_MAX];
    int count;
} EditorCommandRegistry;

void command_registry_init(EditorCommandRegistry *r);
bool command_registry_add(EditorCommandRegistry *r, const char *id, const char *description);
const EditorCommandDef *command_registry_find(const EditorCommandRegistry *r, const char *id);

#endif
