#ifndef PARSER_H
#define PARSER_H

#include "include/ast.h"
#include "include/lexer.h"

ASTNode* ParseProgram(Vector* tokens);
ASTNode* ParseFunction(size_t* index, Vector* tokens);
ASTNode* ParseBlock(size_t* index, Vector* tokens);
ASTNode* ParseStatement(size_t* index, Vector* tokens);
ASTNode* ParseReturnStatement(size_t* index, Vector* tokens);
ASTNode* ParseExpression(size_t* index, Vector* tokens);
ASTNode* ParseLogicalAndExpression(size_t* index, Vector* tokens);
ASTNode* ParseEqualityExpression(size_t* index, Vector* tokens);
ASTNode* ParseRelationalExpression(size_t* index, Vector* tokens);
ASTNode* ParseAdditiveExpression(size_t* index, Vector* tokens);
ASTNode* ParseTerm(size_t* index, Vector* tokens);
ASTNode* ParseFactor(size_t* index, Vector* tokens);

#endif