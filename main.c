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
    char* value;
} Token;

Token* Tokenize(const char* str, size_t fsize, size_t* outCount);

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
    char* buffer = malloc(size + 1);

    if(buffer == NULL) {
        fclose(fptr);
        fprintf(stderr, "Error: not enough memory\n");
        return 1;
    }
    
    fseek(fptr, 0, SEEK_SET);

    fread(buffer, sizeof(char), size, fptr);
    buffer[size] = '\0';

    size_t tokenCount = 0;
    Token* tokens = Tokenize(buffer, size, &tokenCount);
    
    for(int i = 0; i < tokenCount; i++) {
        if(tokens[i].value != NULL) free(tokens[i].value);
    }

    free(tokens);
    free(buffer);
    fclose(fptr);

    return 0;
}

Token* Tokenize(const char* str, size_t fsize, size_t* outCount) {
    Token* tokens = malloc(256 * sizeof(Token));
    int tokCount = 0;

    char tok[256];
    size_t toklen = 0;

    for(int i = 0; i < fsize; i++) {
        char c = str[i];

        if(isalnum(c) || c == '_') {
            tok[toklen++] = c;
            continue;
        }

        if(toklen > 0) {
            tok[toklen] = '\0';

            if(isdigit(tok[0])) {
                char* value = malloc(toklen + 1);
                strcpy(value, tok);
                tokens[tokCount++] = (Token){.type = TOK_INTLIT, .value = value};
            } else {
                if(strcmp(tok, "int") == 0) {
                    tokens[tokCount++] = (Token){.type = TOK_INT, .value = NULL};
                } else if(strcmp(tok, "return") == 0) {
                    tokens[tokCount++] = (Token){.type = TOK_RET, .value = NULL};
                } else {
                    char* value = malloc(toklen + 1);
                    strcpy(value, tok);
                    tokens[tokCount++] = (Token){.type = TOK_ID, .value = value};
                }
            }
            toklen = 0;
        }

        switch(c) {
            case '(': tokens[tokCount++] = (Token){.type = TOK_LPAREN, .value = NULL}; break;
            case ')': tokens[tokCount++] = (Token){.type = TOK_RPAREN, .value = NULL}; break;
            case '{': tokens[tokCount++] = (Token){.type = TOK_LBRACE, .value = NULL}; break;
            case '}': tokens[tokCount++] = (Token){.type = TOK_RBRACE, .value = NULL}; break;
            case ';': tokens[tokCount++] = (Token){.type = TOK_SEMICOLON, .value = NULL}; break;
        }
    }
    
    tokens[tokCount++] = (Token){.type = TOK_EOF, .value = NULL};
    *outCount = tokCount;

    return tokens;
}