#include "include/lexer.h"

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