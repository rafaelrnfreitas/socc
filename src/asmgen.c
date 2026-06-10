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
    } if(node->type == NODE_ADD || node->type == NODE_SUB || 
        node->type == NODE_MUL || node->type == NODE_DIV) {
        GenerateExpression(node->children[0], output);
        fprintf(output, "\tpushq %%rax\n");
        GenerateExpression(node->children[1], output);
        fprintf(output, "\tpopq %%rbx\n");

        if(node->type == NODE_ADD) {
            fprintf(output, "\taddq %%rbx, %%rax\n");
        } else if(node->type == NODE_SUB) {
            fprintf(output, "\tsubq %%rax, %%rbx\n");
            fprintf(output, "\tmovq %%rbx, %%rax\n");
        } else if(node->type == NODE_MUL) {
            fprintf(output, "\timulq %%rbx, %%rax\n");
        } else if(node->type == NODE_DIV) {
            fprintf(output, "\tmovq %%rax, %%rcx\n");
            fprintf(output, "\tmovq %%rbx, %%rax\n");
            fprintf(output, "\tcqto\n");
            fprintf(output, "\tidivq %%rcx\n");
        }
    }
}