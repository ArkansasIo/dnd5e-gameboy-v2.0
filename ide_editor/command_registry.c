#include "command_registry.h"
#include <string.h>

void command_registry_init(EditorCommandRegistry *r) {
    if (!r) return;
    r->count = 0;
}

bool command_registry_add(EditorCommandRegistry *r, const char *id, const char *description) {
    EditorCommandDef *d;
    if (!r || !id || !description || r->count >= COMMAND_MAX) return false;
    d = &r->defs[r->count++];
    strncpy(d->id, id, sizeof(d->id) - 1);
    d->id[sizeof(d->id) - 1] = '\0';
    strncpy(d->description, description, sizeof(d->description) - 1);
    d->description[sizeof(d->description) - 1] = '\0';
    return true;
}

const EditorCommandDef *command_registry_find(const EditorCommandRegistry *r, const char *id) {
    int i;
    if (!r || !id) return NULL;
    for (i = 0; i < r->count; i++) {
        if (strcmp(r->defs[i].id, id) == 0) return &r->defs[i];
    }
    return NULL;
}
