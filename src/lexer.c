#include "include/lexer.h"
#include <ctype.h>
#include <stdlib.h>

Vector Tokenize(const char* str, size_t fsize) {
    Vector tokens;
    VectorInit(&tokens, sizeof(Token));

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
                VectorPush(&(Token){.type = TOK_INTLIT, .value = value}, &tokens);
            } else {
                if(strcmp(tok, "int") == 0) {
                    VectorPush(&(Token){.type = TOK_INT, .value = NULL}, &tokens);
                } else if(strcmp(tok, "return") == 0) {
                    VectorPush(&(Token){.type = TOK_RET, .value = NULL}, &tokens);
                } else {
                    char* value = malloc(toklen + 1);
                    strcpy(value, tok);
                    VectorPush(&(Token){.type = TOK_ID, .value = value}, &tokens);
                }
            }
            toklen = 0;
        }

        switch(c) {
            case '(': VectorPush(&(Token){.type = TOK_OPAREN, .value = NULL}, &tokens); break;
            case ')': VectorPush(&(Token){.type = TOK_CPAREN, .value = NULL}, &tokens); break;
            case '{': VectorPush(&(Token){.type = TOK_OBRACE, .value = NULL}, &tokens); break;
            case '}': VectorPush(&(Token){.type = TOK_CBRACE, .value = NULL}, &tokens); break;
            case ';': VectorPush(&(Token){.type = TOK_SEMICOLON, .value = NULL}, &tokens); break;
        }
    }
    
    VectorPush(&(Token){.type = TOK_EOF, .value = NULL}, &tokens);

    return tokens;
}