#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

/**
 * @brief Translates a single line of Batch code to Bash code.
 * 
 * @param batch_line The input line of Batch code.
 * @param output_file The file pointer to write the Bash code to.
 */
void translate_line(char *batch_line, FILE *output_file);

#endif
