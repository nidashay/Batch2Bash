#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

int batch2Shell(FILE *fptr, FILE *shell);

int main(int argc, char* argv[])
{
	if (argc < 3){
		printf("Usage: %s <path/to/batch/file> <name/of/the/sh/file> \n", argv[0]);
		return 0;
	}

    FILE *batch_file = fopen(argv[1], "r");
    if (batch_file == NULL){
        printf("Could not find file: %s\n", argv[1]);
        return 1;
    }

    FILE *sh_file = fopen(argv[2], "w");
    if (sh_file == NULL){
        printf("Could not write to sh file: %s\n", argv[2]);
        fclose(batch_file);
        return 1;
    }

    // Add shebang
    fprintf(sh_file, "#!/bin/bash\n\n");

    batch2Shell(batch_file, sh_file);

    fclose(batch_file);
    fclose(sh_file);

    printf("Successfully transpiled %s to %s\n", argv[1], argv[2]);

	return 0;
}

int batch2Shell(FILE *fptr, FILE *shell)
{
	char txt_buff[1024];
    while( fgets( txt_buff, sizeof(txt_buff), fptr ) != NULL ){
        translate_line(txt_buff, shell);
    }

	return 0;
}
