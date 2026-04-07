#include "parser.h"
#include <stdio.h>
#include <string.h>

void tokenizer(char bat_code[])
{
    char token[100] = {0};
    size_t j = 0;

    for (size_t i = 0; bat_code[i] != '\0'; i++){
        printf("Buffer Contents: %c\n", bat_code[i]);

        if (bat_code[i] == '\n') {
            continue;
        }

        token[j++] = bat_code[i];

        if (j >= sizeof(token) - 1) {
            break;
        }
    }

    token[j] = '\0';

    printf("Processed token: %s\n", token);
}
