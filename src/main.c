#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "parser.h"

int batch2Shell(FILE *fptr, FILE *shell);

int main(int argc, char* argv[])
{
	if (argc == 1){
		printf("Usage: %s <path/to/batch/file> <name/of/the/sh/file> \n", argv[0]);

		return 0;
	} else {
		FILE *batch_file;
		batch_file = fopen(argv[1], "r");
		FILE *sh_file;
		sh_file = fopen(argv[2], "w");
		if (batch_file == NULL){
			printf("Could not find file: %s\n", argv[1]);

			return 1;
		}
		batch2Shell(batch_file, sh_file);

		fclose(batch_file);
		fclose(sh_file);
	}

	return 0;
}

int batch2Shell(FILE *fptr, FILE *shell)
{

    char buff[1024];
	char txt_buff[1024];
	if (shell == NULL){
		printf("Could not write to sh file\n");

		return 1;
	} else {
		while( fgets( txt_buff, sizeof(txt_buff), fptr ) != NULL ){
			// fprintf(shell, txt_buff);
            Token_t t_parser;
            
            // snprintf(buff, sizeof(buff), txt_buff);

            // printf("%s\n", buff);

            parseToken(&t_parser, "Hello World, This is minecraft guru here\n");

		}

	}

	return 0;
}
