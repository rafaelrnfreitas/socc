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

ASTNode* CreateNode(NodeType type);

#endif
