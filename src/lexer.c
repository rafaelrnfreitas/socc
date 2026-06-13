#include "include/lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static const struct { char a, b; TokenType type; } twoCharTokens[] = {
    {'&', '&', TOK_AMP_AMP},
    {'|', '|', TOK_PIPE_PIPE},
    {'=', '=', TOK_EQUAL_EQUAL},
    {'!', '=', TOK_BANG_EQUAL},
    {'<', '=', TOK_LESS_EQUAL},
    {'>', '=', TOK_GREATER_EQUAL},
    {'<', '<', TOK_LESS_LESS},
    {'>', '>', TOK_GREATER_GREATER},
};

static const struct { char c; TokenType type; } singleCharTokens[] = {
    {'(', TOK_OPAREN},
    {')', TOK_CPAREN},
    {'{', TOK_OBRACE},
    {'}', TOK_CBRACE},
    {';', TOK_SEMICOLON},
    {'+', TOK_PLUS},
    {'-', TOK_MINUS},
    {'*', TOK_STAR},
    {'/', TOK_SLASH},
    {'%', TOK_PERCENT},
    {'!', TOK_BANG},
    {'~', TOK_TILDE},
    {'<', TOK_LESS},
    {'>', TOK_GREATER},
    {'&', TOK_AMP},
    {'|', TOK_PIPE},
    {'^', TOK_CARET},
};

static void EmitToken(char* tok, size_t toklen, Vector* tokens) {
    if(toklen == 0) return;
    tok[toklen] = '\0';

    if(isdigit(tok[0])) {
        char* value = malloc(toklen + 1);
        strcpy(value, tok);
        VectorPush(&(Token){.type = TOK_INTLIT, .value = value}, tokens);
        return;
    }

    if(strcmp(tok, "int") == 0) {
        VectorPush(&(Token){.type = TOK_INT, .value = NULL}, tokens);
    } else if(strcmp(tok, "return") == 0) {
        VectorPush(&(Token){.type = TOK_RET, .value = NULL}, tokens);
    } else {
        char* value = malloc(toklen + 1);
        strcpy(value, tok);
        VectorPush(&(Token){.type = TOK_ID, .value = value}, tokens);
    }
}

Vector Tokenize(const char* str, size_t fsize) {
    Vector tokens;
    VectorInit(&tokens, sizeof(Token));

    char tok[256];
    size_t toklen = 0;

    for(size_t i = 0; i < fsize; i++) {
        char c = str[i];

        if(i + 1 < fsize) {
            char next = str[i + 1];
            int matched = 0;

            for(size_t j = 0; j < sizeof(twoCharTokens) / sizeof(twoCharTokens[0]); j++) {
                if(c == twoCharTokens[j].a && next == twoCharTokens[j].b) {
                    EmitToken(tok, toklen, &tokens);
                    toklen = 0;
                    VectorPush(&(Token){.type = twoCharTokens[j].type, .value = NULL}, &tokens);
                    i++;
                    matched = 1;
                    break;
                }
            }

            if(matched) continue;
        }

        if(isalnum(c) || c == '_') {
            tok[toklen++] = c;
            continue;
        }

        EmitToken(tok, toklen, &tokens);
        toklen = 0;

        for(size_t j = 0; j < sizeof(singleCharTokens) / sizeof(singleCharTokens[0]); j++) {
            if(c == singleCharTokens[j].c) {
                VectorPush(&(Token){.type = singleCharTokens[j].type, .value = NULL}, &tokens);
                break;
            }
        }
    }

    EmitToken(tok, toklen, &tokens);
    VectorPush(&(Token){.type = TOK_EOF, .value = NULL}, &tokens);

    return tokens;
}
