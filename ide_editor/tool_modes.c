#include "tool_modes.h"

static bool apply_command(TileMap *map, EditorCommand cmd, bool use_after) {
    int v = use_after ? cmd.after_value : cmd.before_value;
    if (!map) return false;
    if (cmd.type == CMD_TILE_PAINT || cmd.type == CMD_TILE_FILL) {
        return tilemap_set(map, cmd.x, cmd.y, v);
    }
    return false;
}

bool tool_apply(EditorToolMode mode, TileMap *map, CommandStack *stack, ToolPoint p, int tile_id) {
    EditorCommand cmd;
    int before;
    if (!map || !stack) return false;

    if (mode == TOOL_FILL) {
        before = tilemap_get(map, p.x, p.y);
        if (before < 0) return false;
        if (!tilemap_bucket_fill(map, p.x, p.y, tile_id)) return false;
        cmd.type = CMD_TILE_FILL;
        cmd.x = p.x;
        cmd.y = p.y;
        cmd.before_value = before;
        cmd.after_value = tile_id;
        return command_stack_push(stack, cmd);
    }

    if (mode == TOOL_PAINT || mode == TOOL_ERASE || mode == TOOL_PLACE_OBJECT) {
        before = tilemap_get(map, p.x, p.y);
        if (before < 0) return false;
        cmd.type = CMD_TILE_PAINT;
        cmd.x = p.x;
        cmd.y = p.y;
        cmd.before_value = before;
        cmd.after_value = (mode == TOOL_ERASE) ? 0 : tile_id;
        if (!tilemap_set(map, p.x, p.y, cmd.after_value)) return false;
        return command_stack_push(stack, cmd);
    }

    return true;
}

bool tool_undo_last(TileMap *map, CommandStack *stack) {
    EditorCommand cmd;
    if (!map || !stack) return false;
    if (!command_stack_pop_undo(stack, &cmd)) return false;
    return apply_command(map, cmd, false);
}

bool tool_redo_last(TileMap *map, CommandStack *stack) {
    EditorCommand cmd;
    if (!map || !stack) return false;
    if (!command_stack_pop_redo(stack, &cmd)) return false;
    return apply_command(map, cmd, true);
}
