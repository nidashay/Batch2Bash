#include "parser.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static char *trim_whitespace(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

static void replace_vars(char *line, char *out, size_t out_size) {
    char *p = line;
    char *o = out;
    char *o_end = out + out_size - 1;

    while (*p && o < o_end) {
        if (*p == '%' && *(p + 1) != '%') {
            char *start = p + 1;
            char *end = strchr(start, '%');
            if (end) {
                *o++ = '$';
                while (start < end && o < o_end) {
                    *o++ = *start++;
                }
                p = end + 1;
            } else {
                *o++ = *p++;
            }
        } else if (*p == '%' && *(p + 1) == '%') {
            // Escaped %
            *o++ = '%';
            p += 2;
        } else {
            *o++ = *p++;
        }
    }
    *o = '\0';
}

void translate_line(char *batch_line, FILE *output_file) {
    char *trimmed = trim_whitespace(batch_line);
    if (strlen(trimmed) == 0) {
        fprintf(output_file, "\n");
        return;
    }

    char line_with_vars[2048];
    replace_vars(trimmed, line_with_vars, sizeof(line_with_vars));

    // Convert backslashes to forward slashes in paths (basic heuristic)
    for (int i = 0; line_with_vars[i]; i++) {
        if (line_with_vars[i] == '\\') {
            line_with_vars[i] = '/';
        }
    }

    // Handle comments
    if (strncmp(line_with_vars, "rem ", 4) == 0 || strncmp(line_with_vars, "REM ", 4) == 0) {
        fprintf(output_file, "# %s\n", line_with_vars + 4);
        return;
    }
    if (strncmp(line_with_vars, "::", 2) == 0) {
        fprintf(output_file, "# %s\n", line_with_vars + 2);
        return;
    }

    // Handle @echo off
    if (strcasecmp(line_with_vars, "@echo off") == 0) {
        // Usually ignored in bash
        return;
    }

    // Handle echo
    if (strncasecmp(line_with_vars, "echo ", 5) == 0) {
        fprintf(output_file, "echo %s\n", line_with_vars + 5);
        return;
    }
    if (strcasecmp(line_with_vars, "echo.") == 0) {
        fprintf(output_file, "echo\n");
        return;
    }

    // Handle set
    if (strncasecmp(line_with_vars, "set ", 4) == 0) {
        char *assignment = line_with_vars + 4;
        char *equal = strchr(assignment, '=');
        if (equal) {
            *equal = '\0';
            char *var = trim_whitespace(assignment);
            char *val = trim_whitespace(equal + 1);
            fprintf(output_file, "%s=\"%s\"\n", var, val);
        }
        return;
    }

    // Handle pause
    if (strcasecmp(line_with_vars, "pause") == 0) {
        fprintf(output_file, "read -p \"Press any key to continue...\" -n1 -s\necho\n");
        return;
    }

    // Handle cls
    if (strcasecmp(line_with_vars, "cls") == 0) {
        fprintf(output_file, "clear\n");
        return;
    }

    // Basic command mappings
    if (strncasecmp(line_with_vars, "del ", 4) == 0) {
        fprintf(output_file, "rm %s\n", line_with_vars + 4);
        return;
    }
    if (strncasecmp(line_with_vars, "copy ", 5) == 0) {
        fprintf(output_file, "cp %s\n", line_with_vars + 5);
        return;
    }
    if (strncasecmp(line_with_vars, "move ", 5) == 0) {
        fprintf(output_file, "mv %s\n", line_with_vars + 5);
        return;
    }
    if (strncasecmp(line_with_vars, "md ", 3) == 0) {
        fprintf(output_file, "mkdir %s\n", line_with_vars + 3);
        return;
    }
    if (strncasecmp(line_with_vars, "mkdir ", 6) == 0) {
        fprintf(output_file, "mkdir %s\n", line_with_vars + 6);
        return;
    }
    if (strncasecmp(line_with_vars, "rd ", 3) == 0) {
        fprintf(output_file, "rmdir %s\n", line_with_vars + 3);
        return;
    }
    if (strncasecmp(line_with_vars, "rmdir ", 6) == 0) {
        fprintf(output_file, "rmdir %s\n", line_with_vars + 6);
        return;
    }

    // Handle exit
    if (strcasecmp(line_with_vars, "exit") == 0) {
        fprintf(output_file, "exit\n");
        return;
    }

    // Handle if exist
    if (strncasecmp(line_with_vars, "if exist ", 9) == 0) {
        char *rest = line_with_vars + 9;
        char *space = strchr(rest, ' ');
        if (space) {
            *space = '\0';
            char *file = rest;
            char *command = space + 1;
            fprintf(output_file, "if [ -e \"%s\" ]; then %s; fi\n", file, command);
            return;
        }
    }

    // Fallback: just print the line as is (might be a direct command common to both)
    fprintf(output_file, "%s\n", line_with_vars);
}
