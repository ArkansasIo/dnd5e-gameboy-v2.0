#include "syntax_highlight.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// Simple C/C++ keyword list
static const char *keywords[] = {
    "int", "char", "float", "double", "void", "if", "else", "for", "while", "return", "struct", "typedef", "enum", "const", "static", "include", NULL
};

static bool is_keyword(const char *word) {
    for (int i = 0; keywords[i]; ++i) {
        if (strcmp(word, keywords[i]) == 0) return true;
    }
    return false;
}

void syntax_highlight_line(const char *line, SyntaxHighlightLine *out) {
    int i = 0, j = 0;
    char word[32];
    int wlen = 0;
    while (line[i] && j < EDITOR_FILE_MAX_LINE_LENGTH) {
        if (isalpha(line[i]) || line[i] == '_') {
            wlen = 0;
            int start = i;
            while ((isalnum(line[i]) || line[i] == '_') && wlen < 31) {
                word[wlen++] = line[i++];
            }
            word[wlen] = '\0';
            if (is_keyword(word)) {
                for (int k = start; k < i; ++k) out->tokens[k] = SYNTAX_KEYWORD;
            } else {
                for (int k = start; k < i; ++k) out->tokens[k] = SYNTAX_NONE;
            }
            j = i;
        } else if (line[i] == '"') {
            out->tokens[j++] = SYNTAX_STRING;
            ++i;
            while (line[i] && line[i] != '"' && j < EDITOR_FILE_MAX_LINE_LENGTH) {
                out->tokens[j++] = SYNTAX_STRING;
                ++i;
            }
            if (line[i] == '"') {
                out->tokens[j++] = SYNTAX_STRING;
                ++i;
            }
        } else if (line[i] == '/' && line[i+1] == '/') {
            while (line[i] && j < EDITOR_FILE_MAX_LINE_LENGTH) {
                out->tokens[j++] = SYNTAX_COMMENT;
                ++i;
            }
        } else if (isdigit(line[i])) {
            out->tokens[j++] = SYNTAX_NUMBER;
            ++i;
            while (isdigit(line[i]) && j < EDITOR_FILE_MAX_LINE_LENGTH) {
                out->tokens[j++] = SYNTAX_NUMBER;
                ++i;
            }
        } else {
            out->tokens[j++] = SYNTAX_NONE;
            ++i;
        }
    }
    while (j < EDITOR_FILE_MAX_LINE_LENGTH) out->tokens[j++] = SYNTAX_NONE;
}
