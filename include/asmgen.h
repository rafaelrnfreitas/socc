#ifndef ASMGEN_H
#define ASMGEN_H

#include <stdio.h>
#include "include/ast.h"

void GenerateProgram(ASTNode* node, FILE* output);
void GenerateFunction(ASTNode* node, FILE* output);
void GenerateBlock(ASTNode* node, FILE* output);
void GenerateStatement(ASTNode* node, FILE* output);
void GenerateReturn(ASTNode* node, FILE* output);
void GenerateExpression(ASTNode* node, FILE* output);

#endif