//
// Created by Brandon Jordan on 1/22/24.
//

#include <stdio.h>
#include <stdlib.h>

#include "lexer.c"
#include "interpret.c"

void load_file(FILE *);

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf("usage: bf [FILE]\n");
        return 1;
    }

    FILE *fptr;
    if ((fptr = fopen(argv[1], "r")) == NULL) {
        printf("error: unable to open file '%s'\n", argv[1]);
        return 1;
    }
    filename = argv[1];

    load_file(fptr);
    fclose(fptr);

    lex();
    free(buffer);

    interpret();

    print_operations();
    print_stack();

    free(operations.items);
    free(addresses.items);
    free(stack.items);

    return 0;
}

void load_file(FILE *fptr) {
    fseek(fptr, 0, SEEK_END);
    file_length = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    buffer = malloc(file_length);
    if (!buffer) {
        printf("error: no buffer to read");
        exit(1);
    }
    fread(buffer, 1, file_length, fptr);
}
