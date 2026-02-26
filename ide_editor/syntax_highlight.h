#ifndef SYNTAX_HIGHLIGHT_H
#define SYNTAX_HIGHLIGHT_H

#include "editor_file.h"

// Token types for syntax highlighting
typedef enum {
    SYNTAX_NONE,
    SYNTAX_KEYWORD,
    SYNTAX_TYPE,
    SYNTAX_STRING,
    SYNTAX_COMMENT,
    SYNTAX_NUMBER,
    SYNTAX_PREPROCESSOR,
    SYNTAX_FUNCTION,
    SYNTAX_VARIABLE
} SyntaxTokenType;

// Highlight info for a line
typedef struct {
    SyntaxTokenType tokens[EDITOR_FILE_MAX_LINE_LENGTH];
} SyntaxHighlightLine;

void syntax_highlight_line(const char *line, SyntaxHighlightLine *out);

#endif // SYNTAX_HIGHLIGHT_H
