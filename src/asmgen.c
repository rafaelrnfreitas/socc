#include "include/asmgen.h"
#include <stdlib.h>

void GenerateProgram(ASTNode* node, FILE* output) {
    for(int i = 0; i < node->childCount; i++) {
        GenerateFunction(node->children[i], output);
    }
}

void GenerateFunction(ASTNode* node, FILE* output) {
    fprintf(output, ".global %s\n", node->id);
    fprintf(output, "\n%s:\n", node->id);
    GenerateBlock(node->children[0], output);
}

void GenerateBlock(ASTNode* node, FILE* output) {
    for(int i = 0; i < node->childCount; i++) {
        GenerateStatement(node->children[i], output);
    }
}

void GenerateStatement(ASTNode* node, FILE* output) {
    if(node->type == NODE_RETURN) {
        GenerateReturn(node, output);
    }
}

void GenerateReturn(ASTNode* node, FILE* output) {
    GenerateExpression(node->children[0], output);
    fprintf(output, "\tret\n");
}

void GenerateExpression(ASTNode* node, FILE* output) {
    if(node->type == NODE_NUMBER) {
        fprintf(output, "\tmovq $%d, %%rax\n", node->intLit);
    } else if(node->type == NODE_NEG || node->type == NODE_BITNOT) {
        GenerateExpression(node->children[0], output);
        fprintf(output, "\tneg %%rax\n");
    } else if(node->type == NODE_NOT) {
        GenerateExpression(node->children[0], output);
        fprintf(output, "\tcmpq $0, %%rax\n");
        fprintf(output, "\tsete %%al\n");
        fprintf(output, "\tmovzbq %%al, %%rax\n");
    }
}