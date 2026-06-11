#ifndef AST_H
#define AST_H

#include <stddef.h>

typedef enum {
    NODE_PROGRAM,
    NODE_FUNCTION,
    NODE_BLOCK,
    NODE_NUMBER,
    NODE_NEG,
    NODE_NOT,
    NODE_BITNOT,
	NODE_ADD,
	NODE_SUB,
	NODE_MUL,
	NODE_DIV,
    NODE_LOG_AND,
    NODE_LOG_OR,
    NODE_EQUAL,
    NODE_NOT_EQUAL,
    NODE_LESS,
    NODE_LESS_EQUAL,
    NODE_GREATER,
    NODE_GREATER_EQUAL,
    NODE_RETURN,
} NodeType;

typedef struct ASTNode {
    NodeType type;
    struct ASTNode** children;
    size_t childCount;

    union {
        int intLit;
        char* id;
    };
} ASTNode;

#endif
