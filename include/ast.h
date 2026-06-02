#ifndef AST_H
#define AST_H

#include <stddef.h>

typedef enum {
    NODE_NUMBER,
    NODE_RETURN,
    NODE_FUNCTION,
    NODE_BLOCK
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