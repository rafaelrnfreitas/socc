#include "include/lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

Vector Tokenize(const char* str, size_t fsize) {
    Vector tokens;
    VectorInit(&tokens, sizeof(Token));

    char tok[256];
    size_t toklen = 0;

    for(int i = 0; i < fsize; i++) {
        char c = str[i];

        if(i + 1 < fsize) {
            char next = str[i + 1];

            if(c == '&' && next == '&') {
                VectorPush(&(Token){.type = TOK_AMP_AMP, .value = NULL}, &tokens);
                i++;
                continue;
            }

            if(c == '|' && next == '|') {
                VectorPush(&(Token){.type = TOK_PIPE_PIPE, .value = NULL}, &tokens);
                i++;
                continue;
            }

            if(c == '=' && next == '=') {
                VectorPush(&(Token){.type = TOK_EQUAL_EQUAL, .value = NULL}, &tokens);
                i++;
                continue;
            }

            if(c == '!' && next == '=') {
                VectorPush(&(Token){.type = TOK_BANG_EQUAL, .value = NULL}, &tokens);
                i++;
                continue;
            }

            if(c == '<' && next == '=') {
                VectorPush(&(Token){.type = TOK_LESS_EQUAL, .value = NULL}, &tokens);
                i++;
                continue;
            }

            if(c == '>' && next == '=') {
                VectorPush(&(Token){.type = TOK_GREATER_EQUAL, .value = NULL}, &tokens);
                i++;
                continue;
            }
        }

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

            case '+': VectorPush(&(Token){.type = TOK_PLUS, .value = NULL}, &tokens); break;
            case '-': VectorPush(&(Token){.type = TOK_MINUS, .value = NULL}, &tokens); break;
            case '*': VectorPush(&(Token){.type = TOK_STAR, .value = NULL}, &tokens); break;
            case '/': VectorPush(&(Token){.type = TOK_SLASH, .value = NULL}, &tokens); break;

            case '!': VectorPush(&(Token){.type = TOK_BANG, .value = NULL}, &tokens); break;
            case '~': VectorPush(&(Token){.type = TOK_TILDE, .value = NULL}, &tokens); break;
            case '<': VectorPush(&(Token){.type = TOK_LESS, .value = NULL}, &tokens); break;
            case '>': VectorPush(&(Token){.type = TOK_GREATER, .value = NULL}, &tokens); break;
        }
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
    }

    VectorPush(&(Token){.type = TOK_EOF, .value = NULL}, &tokens);

    return tokens;
}