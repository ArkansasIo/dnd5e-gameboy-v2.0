#include "command_stack.h"

void command_stack_init(CommandStack *stack) {
    if (!stack) return;
    stack->undo_top = 0;
    stack->redo_top = 0;
}

bool command_stack_push(CommandStack *stack, EditorCommand cmd) {
    if (!stack || stack->undo_top >= CMD_STACK_MAX) return false;
    stack->undo_stack[stack->undo_top++] = cmd;
    stack->redo_top = 0;
    return true;
}

bool command_stack_pop_undo(CommandStack *stack, EditorCommand *out) {
    if (!stack || !out || stack->undo_top <= 0) return false;
    *out = stack->undo_stack[--stack->undo_top];
    if (stack->redo_top < CMD_STACK_MAX) stack->redo_stack[stack->redo_top++] = *out;
    return true;
}

bool command_stack_pop_redo(CommandStack *stack, EditorCommand *out) {
    if (!stack || !out || stack->redo_top <= 0) return false;
    *out = stack->redo_stack[--stack->redo_top];
    if (stack->undo_top < CMD_STACK_MAX) stack->undo_stack[stack->undo_top++] = *out;
    return true;
}

void command_stack_clear_redo(CommandStack *stack) {
    if (!stack) return;
    stack->redo_top = 0;
}
