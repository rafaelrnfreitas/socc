#include "include/ast.h"
#include <stdlib.h>
#include <stdio.h>

ASTNode* CreateNode(NodeType type) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->childCount = 0;
    node->children = NULL;
    node->type = type;

    return node;
}

void PrintAST(ASTNode* node, int indent) {
    if(!node) return;

    for(int i = 0; i < indent; i++) printf("  ");

    printf("Node type: %d\n", node->type);

    for(size_t i = 0; i < node->childCount; i++) {
        PrintAST(node->children[i], indent + 1);
    }
}