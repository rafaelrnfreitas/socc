#include <stdio.h>
#include <stdlib.h>
#include "include/lexer.h"
#include "include/ast.h"
#include "include/parser.h"
#include "include/asmgen.h"

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Error: missing input file\n");
        return 1;
    } else if(argc < 3) {
        fprintf(stderr, "Error: missing output file\n");
        return 1;
    }

    FILE* fptr = fopen(argv[1], "r");
    if(fptr == NULL) {
        fprintf(stderr, "Error: could not open file\n");
        return 1;
    }

    if(fseek(fptr, 0, SEEK_END) != 0) {
        fclose(fptr);
        fprintf(stderr, "Error: could not get file size\n");
        return 1;
    }
    
    size_t size = ftell(fptr);
    char* buffer = malloc(size + 1);

    if(buffer == NULL) {
        fclose(fptr);
        fprintf(stderr, "Error: not enough memory\n");
        return 1;
    }
    
    fseek(fptr, 0, SEEK_SET);

    fread(buffer, sizeof(char), size, fptr);
    buffer[size] = '\0';

    Vector tokens = Tokenize(buffer, size);

    ASTNode* root = ParseProgram(&tokens);

    FILE* output = fopen(argv[2], "w");
    GenerateProgram(root, output);
    fclose(output);
    
    VectorDestroy(&tokens);
    
    free(buffer);
    fclose(fptr);

    return 0;
}