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

int IsConstant(ASTNode* node) {
    return node->type == NODE_NUMBER;
}

int IsUnaryOperation(ASTNode* node) {
    return node->type == NODE_NEG ||
           node->type == NODE_NOT ||
           node->type == NODE_BIT_NOT;
}

int IsBinaryOperation(ASTNode* node) {
    return node->type == NODE_ADD || 
           node->type == NODE_SUB || 
           node->type == NODE_MUL ||
           node->type == NODE_DIV ||
		   node->type == NODE_MOD ||
           node->type == NODE_EQUAL ||
           node->type == NODE_NOT_EQUAL ||
           node->type == NODE_LESS ||
           node->type == NODE_LESS_EQUAL ||
           node->type == NODE_GREATER ||
           node->type == NODE_GREATER_EQUAL ||
           node->type == NODE_BIT_OR ||
           node->type == NODE_BIT_AND ||
           node->type == NODE_BIT_XOR ||
           node->type == NODE_SHIFT_RIGHT ||
           node->type == NODE_SHIFT_LEFT;
}

int IsLogicalOperation(ASTNode* node) {
    return node->type == NODE_LOG_AND || 
           node->type == NODE_LOG_OR;
}

void GenerateExpression(ASTNode* node, FILE* output) {
    if(IsConstant(node)) {
        fprintf(output, "\tmovq $%d, %%rax\n", node->intLit);
    } else if(IsUnaryOperation(node)) {
        if(node->type == NODE_NEG) {
            GenerateExpression(node->children[0], output);
            fprintf(output, "\tneg %%rax\n");
        } else if(node->type == NODE_BIT_NOT) {
            GenerateExpression(node->children[0], output);
            fprintf(output, "\tnot %%rax\n");
        } else if(node->type == NODE_NOT) {
            GenerateExpression(node->children[0], output);
            fprintf(output, "\tcmpq $0, %%rax\n");
            fprintf(output, "\tsete %%al\n");
            fprintf(output, "\tmovzbq %%al, %%rax\n");
        }
    } else if(IsBinaryOperation(node)) {
        GenerateExpression(node->children[0], output);
        fprintf(output, "\tpushq %%rax\n");
        GenerateExpression(node->children[1], output);
        fprintf(output, "\tpopq %%rbx\n");

        switch(node->type) {
            case NODE_ADD: 
                fprintf(output, "\taddq %%rbx, %%rax\n");
                break;
            case NODE_SUB:
                fprintf(output, "\tsubq %%rax, %%rbx\n");
                fprintf(output, "\tmovq %%rbx, %%rax\n");
                break;
            case NODE_MUL:
                fprintf(output, "\timulq %%rbx, %%rax\n");
                break;
            case NODE_DIV:
                fprintf(output, "\tmovq %%rax, %%rcx\n");
                fprintf(output, "\tmovq %%rbx, %%rax\n");
                fprintf(output, "\tcqto\n");
                fprintf(output, "\tidivq %%rcx\n");
                break;
            case NODE_MOD:
                fprintf(output, "\tmovq %%rax, %%rcx\n");
                fprintf(output, "\tmovq %%rbx, %%rax\n");
                fprintf(output, "\tcqto\n");
                fprintf(output, "\tidivq %%rcx\n");
                fprintf(output, "\tmovq %%rdx, %%rax\n");
                break;
            case NODE_EQUAL:
                fprintf(output, "\tcmpq %%rax, %%rbx\n");
                fprintf(output, "\tmovq $0, %%rax\n");
                fprintf(output, "\tsete %%al\n");
                break;
            case NODE_NOT_EQUAL:
                fprintf(output, "\tcmpq %%rax, %%rbx\n");
                fprintf(output, "\tmovq $0, %%rax\n");
                fprintf(output, "\tsetne %%al\n");
                break;
            case NODE_LESS:
                fprintf(output, "\tcmpq %%rax, %%rbx\n");
                fprintf(output, "\tmovq $0, %%rax\n");
                fprintf(output, "\tsetl %%al\n");
                break;
            case NODE_LESS_EQUAL:
                fprintf(output, "\tcmpq %%rax, %%rbx\n");
                fprintf(output, "\tmovq $0, %%rax\n");
                fprintf(output, "\tsetle %%al\n");
                break;
            case NODE_GREATER:
                fprintf(output, "\tcmpq %%rax, %%rbx\n");
                fprintf(output, "\tmovq $0, %%rax\n");
                fprintf(output, "\tsetg %%al\n");
                break;
            case NODE_BIT_OR:
                fprintf(output, "\torq %%rbx, %%rax\n");
                break;
            case NODE_BIT_AND:
                fprintf(output, "\tandq %%rbx, %%rax\n");
                break;
            case NODE_BIT_XOR:
                fprintf(output, "\txorq %%rbx, %%rax\n");
                break;
            case NODE_SHIFT_RIGHT:
                fprintf(output, "\tmovq %%rax, %%rcx\n");
                fprintf(output, "\tmovq %%rbx, %%rax\n");
                fprintf(output, "\tsarq %%cl, %%rax\n");
                break;
            case NODE_SHIFT_LEFT:
                fprintf(output, "\tmovq %%rax, %%rcx\n");
                fprintf(output, "\tmovq %%rbx, %%rax\n");
                fprintf(output, "\tsalq %%cl, %%rax\n");
                break;
        }
    } else if(IsLogicalOperation(node)) {
        static int label_id = 0;
        int id = label_id++;
        GenerateExpression(node->children[0], output);

        if(node->type == NODE_LOG_AND) {
            fprintf(output, "\tcmpq $0, %%rax\n");
            fprintf(output, "\tje _false_%d\n", id);
            GenerateExpression(node->children[1], output);
            fprintf(output, "\tcmpq $0, %%rax\n");
            fprintf(output, "\tje _false_%d\n", id);
            fprintf(output, "\tmovq $1, %%rax\n");
            fprintf(output, "\tjmp _end_%d\n", id);
            fprintf(output, "_false_%d:\n", id);
            fprintf(output, "\tmovq $0, %%rax\n");
        } else {
            fprintf(output, "\tcmpq $0, %%rax\n");
            fprintf(output, "\tjne _true_%d\n", id);
            GenerateExpression(node->children[1], output);
            fprintf(output, "\tcmpq $0, %%rax\n");
            fprintf(output, "\tjne _true_%d\n", id);
            fprintf(output, "\tmovq $0, %%rax\n");
            fprintf(output, "\tjmp _end_%d\n", id);
            fprintf(output, "_true_%d:\n", id);
            fprintf(output, "\tmovq $1, %%rax\n");
        }
        fprintf(output, "_end_%d:\n", id);
    }
}
