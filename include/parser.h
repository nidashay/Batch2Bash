#pragma once

#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

typedef struct{

    const char* data;

    size_t size;
    

} Token_t;

Token_t tokenizer(char bat_code[]);

void parseToken(Token_t *token, char* str);

#endif 
