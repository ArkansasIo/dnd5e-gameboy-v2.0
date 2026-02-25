#ifndef TOOL_MODES_H
#define TOOL_MODES_H

#include <stdbool.h>
#include "map_editor.h"
#include "command_stack.h"

typedef enum {
    TOOL_SELECT,
    TOOL_PAINT,
    TOOL_FILL,
    TOOL_ERASE,
    TOOL_PLACE_OBJECT
} EditorToolMode;

typedef struct {
    int x;
    int y;
} ToolPoint;

bool tool_apply(EditorToolMode mode, TileMap *map, CommandStack *stack, ToolPoint p, int tile_id);
bool tool_undo_last(TileMap *map, CommandStack *stack);
bool tool_redo_last(TileMap *map, CommandStack *stack);

#endif
