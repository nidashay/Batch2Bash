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
        if (*p == '%') {
            if (isdigit((unsigned char)*(p + 1))) {
                // Handle %1, %2, etc.
                *o++ = '$';
                *o++ = *(p + 1);
                p += 2;
            } else if (*(p + 1) == '~' && *(p + 2) == 'd' && *(p + 3) == 'p' && *(p + 4) == '0') {
                // Handle %~dp0
                if (o + 17 < o_end) {
                    strcpy(o, "$(dirname \"$0\")");
                    o += 16;
                }
                p += 5;
            } else if (*(p + 1) == '%') {
                // Escaped %
                *o++ = '%';
                p += 2;
            } else {
                char *start = p + 1;
                char *end = strchr(start, '%');
                if (end) {
                    size_t len = end - start;
                    if (len == 2 && strncasecmp(start, "CD", 2) == 0) {
                        if (o + 5 < o_end) {
                            strcpy(o, "$PWD");
                            o += 4;
                        }
                    } else {
                        *o++ = '$';
                        while (start < end && o < o_end) {
                            *o++ = *start++;
                        }
                    }
                    p = end + 1;
                } else {
                    *o++ = *p++;
                }
            }
        } else {
            *o++ = *p++;
        }
    }
    *o = '\0';
}

static char *smart_quote(char *str, char *buf, size_t buf_size) {
    if (str[0] == '"' && str[strlen(str) - 1] == '"') {
        strncpy(buf, str, buf_size);
    } else {
        snprintf(buf, buf_size, "\"%s\"", str);
    }
    return buf;
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
    // Only if not in a string? For now, keep it simple.
    for (int i = 0; line_with_vars[i]; i++) {
        if (line_with_vars[i] == '\\') {
            line_with_vars[i] = '/';
        }
    }

    // Handle comments
    if (strncasecmp(line_with_vars, "rem ", 4) == 0) {
        fprintf(output_file, "# %s\n", line_with_vars + 4);
        return;
    }
    if (strncmp(line_with_vars, "::", 2) == 0) {
        fprintf(output_file, "# %s\n", line_with_vars + 2);
        return;
    }

    // Handle @echo off
    if (strcasecmp(line_with_vars, "@echo off") == 0 || strcasecmp(line_with_vars, "@echo on") == 0) {
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
            fprintf(output_file, "%s=%s\n", var, smart_quote(val, (char[256]){0}, 256));
        } else {
            // Probably set /p or something not fully supported, or just 'set' to list vars
            fprintf(output_file, "# Unsupported set command: %s\n", line_with_vars);
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

    // Handle cd / chdir
    if (strncasecmp(line_with_vars, "cd ", 3) == 0) {
        fprintf(output_file, "cd %s\n", line_with_vars + 3);
        return;
    }
    if (strncasecmp(line_with_vars, "chdir ", 6) == 0) {
        fprintf(output_file, "cd %s\n", line_with_vars + 6);
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
    if (strncasecmp(line_with_vars, "md ", 3) == 0 || strncasecmp(line_with_vars, "mkdir ", 6) == 0) {
        char *dir = (strncasecmp(line_with_vars, "md ", 3) == 0) ? line_with_vars + 3 : line_with_vars + 6;
        fprintf(output_file, "mkdir -p %s\n", dir);
        return;
    }
    if (strncasecmp(line_with_vars, "rd ", 3) == 0 || strncasecmp(line_with_vars, "rmdir ", 6) == 0) {
        char *dir = (strncasecmp(line_with_vars, "rd ", 3) == 0) ? line_with_vars + 3 : line_with_vars + 6;
        fprintf(output_file, "rmdir %s\n", dir);
        return;
    }

    // Handle exit
    if (strcasecmp(line_with_vars, "exit") == 0) {
        fprintf(output_file, "exit\n");
        return;
    }

    // Handle if commands
    if (strncasecmp(line_with_vars, "if ", 3) == 0) {
        char *rest = line_with_vars + 3;
        if (strncasecmp(rest, "not exist ", 10) == 0) {
            char *file_cmd = rest + 10;
            char *space = strchr(file_cmd, ' ');
            if (space) {
                *space = '\0';
                fprintf(output_file, "if [ ! -e \"%s\" ]; then %s; fi\n", file_cmd, space + 1);
                return;
            }
        } else if (strncasecmp(rest, "exist ", 6) == 0) {
            char *file_cmd = rest + 6;
            char *space = strchr(file_cmd, ' ');
            if (space) {
                *space = '\0';
                fprintf(output_file, "if [ -e \"%s\" ]; then %s; fi\n", file_cmd, space + 1);
                return;
            }
        } else {
            // Handle if "a"=="b"
            char *eq = strstr(rest, "==");
            if (eq) {
                *eq = '\0';
                char *left = trim_whitespace(rest);
                char *right_cmd = eq + 2;
                char *space = strchr(right_cmd, ' ');
                if (space) {
                    *space = '\0';
                    char *right = trim_whitespace(right_cmd);
                    char q_left[512], q_right[512];
                    fprintf(output_file, "if [ %s = %s ]; then %s; fi\n", 
                            smart_quote(left, q_left, sizeof(q_left)), 
                            smart_quote(right, q_right, sizeof(q_right)), 
                            space + 1);
                    return;
                }
            }
        }
    }

    // Fallback: just print the line as is (might be a direct command common to both)
    fprintf(output_file, "%s\n", line_with_vars);
}
