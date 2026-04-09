#include "parser.h"
#include <stdio.h>
#include <string.h>

void tokenizer(char bat_code[]) {
    char token[100];
    int j = 0;
    
    char full_str[1024];


    for (int i = 0; i < strlen(bat_code); i++) {

        if (bat_code[i] == ' ' || bat_code[i] == '\n') {
            if (j > 0) {
                token[j] = '\0';   // end string
                printf("Token: %s\n", token);
                j = 0;             // reset buffer
            }
        } else {
            token[j++] = bat_code[i]; // build string
        }
    }

    if (j > 0) {
        token[j] = '\0';
        printf("Token: %s\n", token);
    }

    snprintf(full_str, sizeof(full_str), token);

    printf("Full str: %s\n", full_str);
}
