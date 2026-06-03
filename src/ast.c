#include "include/ast.h"

ASTNode* CreateNode(NodeType type) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->childCount = 0;
    node->children = NULL;
    node->type = type;

    return node;
}