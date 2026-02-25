#ifndef COMMAND_STACK_H
#define COMMAND_STACK_H

#include <stdbool.h>

#define CMD_STACK_MAX 512

typedef enum {
    CMD_TILE_PAINT,
    CMD_TILE_FILL,
    CMD_ASSET_RENAME,
    CMD_UNKNOWN
} CommandType;

typedef struct {
    CommandType type;
    int x;
    int y;
    int before_value;
    int after_value;
} EditorCommand;

typedef struct {
    EditorCommand undo_stack[CMD_STACK_MAX];
    EditorCommand redo_stack[CMD_STACK_MAX];
    int undo_top;
    int redo_top;
} CommandStack;

void command_stack_init(CommandStack *stack);
bool command_stack_push(CommandStack *stack, EditorCommand cmd);
bool command_stack_pop_undo(CommandStack *stack, EditorCommand *out);
bool command_stack_pop_redo(CommandStack *stack, EditorCommand *out);
void command_stack_clear_redo(CommandStack *stack);

#endif
