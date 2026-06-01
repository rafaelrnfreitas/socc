#ifndef LEXER_H
#define LEXER_H

#include <string.h>

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

#endif