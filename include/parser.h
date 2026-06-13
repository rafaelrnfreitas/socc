#ifndef PARSER_H
#define PARSER_H

#include "include/ast.h"
#include "include/lexer.h"

ASTNode* ParseProgram(Vector* tokens);
ASTNode* ParseFunction(size_t* index, Vector* tokens);
ASTNode* ParseBlock(size_t* index, Vector* tokens);
ASTNode* ParseStatement(size_t* index, Vector* tokens);
ASTNode* ParseReturnStatement(size_t* index, Vector* tokens);
ASTNode* ParseLogicalOrExpr(size_t* index, Vector* tokens);
ASTNode* ParseLogicalAndExpr(size_t* index, Vector* tokens);
ASTNode* ParseBitwiseOrExpr(size_t* index, Vector* tokens);
ASTNode* ParseBitwiseXorExpr(size_t* index, Vector* tokens);
ASTNode* ParseBitwiseAndExpr(size_t* index, Vector* tokens);
ASTNode* ParseEqualityExpr(size_t* index, Vector* tokens);
ASTNode* ParseRelationalExpr(size_t* index, Vector* tokens);
ASTNode* ParseShiftExpr(size_t* index, Vector* tokens);
ASTNode* ParseAdditiveExpr(size_t* index, Vector* tokens);
ASTNode* ParseMultiplicativeExpr(size_t* index, Vector* tokens);
ASTNode* ParsePrimaryExpr(size_t* index, Vector* tokens);

#endif
