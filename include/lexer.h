#ifndef LEXER_H
#define LEXER_H

#include <string.h>
#include "include/vector.h"

typedef enum {
    TOK_INT,
    TOK_ID,
    TOK_OPAREN,
    TOK_CPAREN,
    TOK_OBRACE,
    TOK_CBRACE,
    TOK_RET,
    TOK_SEMICOLON,
    TOK_INTLIT,
	TOK_PLUS,
    TOK_MINUS,
	TOK_STAR,
	TOK_SLASH,
    TOK_TILDE,
    TOK_BANG,
	TOK_AMP_AMP,
	TOK_PIPE_PIPE,
	TOK_EQUAL_EQUAL,
	TOK_BANG_EQUAL,
	TOK_LESS,
	TOK_LESS_EQUAL,
	TOK_GREATER,
	TOK_GREATER_EQUAL,
    TOK_EOF
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;

Vector Tokenize(const char* str, size_t fsize);

#endif
