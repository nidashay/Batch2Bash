#include "parser.h"
#include <stdio.h>
#include <string.h>


void paerseToken(Token_t *token, char* str)
{

    token->data = str;
    token->size = sizeof(str);

    while ( token->data != NULL ){

        printf("%s", token->data);

    }


    // return token;

}
