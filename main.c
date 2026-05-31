#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOK_INT,
    TOK_ID,
    TOK_LPAREN,
    TOK_RPAREN,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_RET,
    TOK_SEMICOLON,
    TOK_INTLIT,
    TOK_EOF
} TokenType;

typedef struct {
    TokenType type;
    const char* value;
} Token;

Token* Tokenize(const char* str, size_t fsize);

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Error: missing input file\n");
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
    char* buffer = malloc(size);

    if(buffer == NULL) {
        fclose(fptr);
        fprintf(stderr, "Error: not enough memory\n");
        return 1;
    }
    
    fseek(fptr, 0, SEEK_SET);

    fread(buffer, sizeof(char), size, fptr);
    Tokenize(buffer, size);

    free(buffer);
    fclose(fptr);

    return 0;
}

Token* Tokenize(const char* str, size_t fsize) {
    for(int i = 0; i < fsize; i++) {
        char c = str[i];
        printf("%c\n", c);
    }
}