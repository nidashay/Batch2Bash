#pragma once

#ifndef PARSER_H
#define PARSER_H

typedef struct{

    char temp_buff[1024];


} Parser_t;

void tokenizer(char bat_code[]);

#endif 
