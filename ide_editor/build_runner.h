#ifndef BUILD_RUNNER_H
#define BUILD_RUNNER_H

#define BUILD_OUTPUT_MAX_LINES 512
#define BUILD_OUTPUT_LINE_LENGTH 256

// Build/run output buffer
typedef struct {
    char lines[BUILD_OUTPUT_MAX_LINES][BUILD_OUTPUT_LINE_LENGTH];
    int line_count;
    int last_exit_code;
} BuildOutputBuffer;

// Build/run integration API
void build_run_project(const char *build_cmd, BuildOutputBuffer *output);
void build_output_draw(const BuildOutputBuffer *output);

#endif // BUILD_RUNNER_H
