#include "build_runner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void build_run_project(const char *build_cmd, BuildOutputBuffer *output) {
    output->line_count = 0;
    output->last_exit_code = 0;
    FILE *fp = popen(build_cmd, "r");
    if (!fp) {
        snprintf(output->lines[0], BUILD_OUTPUT_LINE_LENGTH, "Failed to run: %s", build_cmd);
        output->line_count = 1;
        output->last_exit_code = -1;
        return;
    }
    char line[BUILD_OUTPUT_LINE_LENGTH];
    while (fgets(line, sizeof(line), fp) && output->line_count < BUILD_OUTPUT_MAX_LINES) {
        strncpy(output->lines[output->line_count], line, BUILD_OUTPUT_LINE_LENGTH-1);
        output->lines[output->line_count][BUILD_OUTPUT_LINE_LENGTH-1] = '\0';
        output->line_count++;
    }
    output->last_exit_code = pclose(fp);
}

void build_output_draw(const BuildOutputBuffer *output) {
    printf("--- Build/Run Output (exit code: %d) ---\n", output->last_exit_code);
    for (int i = 0; i < output->line_count; ++i) {
        printf("%s", output->lines[i]);
    }
}
