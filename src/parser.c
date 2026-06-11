#include "include/parser.h"

ASTNode* ParseProgram(Vector* tokens) {
    ASTNode* program = malloc(sizeof(ASTNode));
    program->children = NULL;
    program->childCount = 0;
    size_t index = 0;
    
    while(1) {
        Token* token = VectorGetAt(index, tokens);
        if(token->type == TOK_EOF) break;

        ASTNode* function = ParseFunction(&index, tokens);
        if(!function) break;

        program->childCount++;
        program->children = realloc(program->children, program->childCount * sizeof(ASTNode*));
        program->children[program->childCount - 1] = function;
    }

    return program;
}

ASTNode* ParseFunction(size_t* index, Vector* tokens) {
    ASTNode* function = malloc(sizeof(ASTNode));
    function->childCount = 0;

    Token* token = VectorGetAt(*index, tokens);
    if(token->type != TOK_INT) return NULL;
    (*index)++;

    token = VectorGetAt(*index, tokens);
    if(token->type != TOK_ID) return NULL;
    (*index)++;

    function->id = token->value;

    token = VectorGetAt(*index, tokens);
    if(token->type != TOK_OPAREN) return NULL;
    (*index)++;

    token = VectorGetAt(*index, tokens);
    if(token->type != TOK_CPAREN) return NULL;
    (*index)++;

    ASTNode* block = ParseBlock(index, tokens);
    if(!block) return NULL;

    function->childCount++;
    function->children = malloc(function->childCount * sizeof(ASTNode*));
    function->children[function->childCount - 1] = block;

    return function;
}

ASTNode* ParseBlock(size_t* index, Vector* tokens) {
    ASTNode* block = malloc(sizeof(ASTNode));
    block->children = NULL;
    block->childCount = 0;

    Token* token = VectorGetAt(*index, tokens);
    if(token->type != TOK_OBRACE) return NULL;
    (*index)++;

    while(1) {
        token = VectorGetAt(*index, tokens);

        if(token->type == TOK_CBRACE) break;
        if(token->type == TOK_EOF) return NULL;

        ASTNode* statement = ParseStatement(index, tokens);
        if(!statement) return NULL;

        block->childCount++;
        block->children = realloc(block->children, block->childCount * sizeof(ASTNode*));
        block->children[block->childCount - 1] = statement;
    }

    (*index)++;

    return block;
}

ASTNode* ParseStatement(size_t* index, Vector* tokens) {
    Token* token = VectorGetAt(*index, tokens);

    if(token->type == TOK_RET) {
        return ParseReturnStatement(index, tokens);
    }

    return NULL;
}

ASTNode* ParseReturnStatement(size_t* index, Vector* tokens) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_RETURN;
    (*index)++;

    ASTNode* expr = ParseExpression(index, tokens);
    if(!expr) return NULL;

    node->childCount = 1;
    node->children = malloc(sizeof(ASTNode*));
    node->children[0] = expr;

    Token* token = VectorGetAt(*index, tokens);
    if(token->type != TOK_SEMICOLON) return NULL;

    (*index)++;

    return node;
}

ASTNode* ParseExpression(size_t* index, Vector* tokens) {
    ASTNode* left = ParseLogicalAndExpression(index, tokens);
    if(!left) return NULL;

    while(1) {
        Token* token = VectorGetAt(*index, tokens);
        if(!token) break;

        NodeType op;

        if(token->type == TOK_PIPE_PIPE) op = NODE_LOG_OR;
        else break;

        (*index)++;

        ASTNode* right = ParseLogicalAndExpression(index, tokens);
        if(!right) return NULL;

        ASTNode* node = malloc(sizeof(ASTNode));
        node->childCount = 2;
        node->children = malloc(node->childCount * sizeof(ASTNode*));
        node->type = op;
        node->children[0] = left;
        node->children[1] = right;

        left = node;
    }

    return left;
}

ASTNode* ParseLogicalAndExpression(size_t* index, Vector* tokens) {
    ASTNode* left = ParseEqualityExpression(index, tokens);
    if(!left) return NULL;

    while(1) {
        Token* token = VectorGetAt(*index, tokens);
        if(!token) break;

        NodeType op;

        if(token->type == TOK_AMP_AMP) op = NODE_LOG_AND;
        else break;

        (*index)++;

        ASTNode* right = ParseEqualityExpression(index, tokens);
        if(!right) return NULL;

        ASTNode* node = malloc(sizeof(ASTNode));
        node->childCount = 2;
        node->children = malloc(node->childCount * sizeof(ASTNode*));
        node->type = op;
        node->children[0] = left;
        node->children[1] = right;

        left = node;
    }

    return left;
}

ASTNode* ParseEqualityExpression(size_t* index, Vector* tokens) {
    ASTNode* left = ParseRelationalExpression(index, tokens);
    if(!left) return NULL;

    while(1) {
        Token* token = VectorGetAt(*index, tokens);
        if(!token) break;

        NodeType op;

        if(token->type == TOK_EQUAL_EQUAL) op = NODE_EQUAL;
        else if(token->type == TOK_BANG_EQUAL) op = NODE_NOT_EQUAL;
        else break;

        (*index)++;

        ASTNode* right = ParseRelationalExpression(index, tokens);
        if(!right) return NULL;

        ASTNode* node = malloc(sizeof(ASTNode));
        node->childCount = 2;
        node->children = malloc(node->childCount * sizeof(ASTNode*));
        node->type = op;
        node->children[0] = left;
        node->children[1] = right;

        left = node;
    }

    return left;
}

ASTNode* ParseRelationalExpression(size_t* index, Vector* tokens) {
    ASTNode* left = ParseAdditiveExpression(index, tokens);
    if(!left) return NULL;

    while(1) {
        Token* token = VectorGetAt(*index, tokens);
        if(!token) break;

        NodeType op;

        if(token->type == TOK_LESS) op = NODE_LESS;
        else if(token->type == TOK_LESS_EQUAL) op = NODE_LESS_EQUAL;
        else if(token->type == TOK_GREATER) op = NODE_GREATER;
        else if(token->type == TOK_GREATER_EQUAL) op = NODE_GREATER_EQUAL;
        else break;

        (*index)++;

        ASTNode* right = ParseAdditiveExpression(index, tokens);
        if(!right) return NULL;

        ASTNode* node = malloc(sizeof(ASTNode));
        node->childCount = 2;
        node->children = malloc(node->childCount * sizeof(ASTNode*));
        node->type = op;
        node->children[0] = left;
        node->children[1] = right;

        left = node;
    }

    return left;
}

ASTNode* ParseAdditiveExpression(size_t* index, Vector* tokens) {
    ASTNode* left = ParseTerm(index, tokens);
    if(!left) return NULL;

    while(1) {
        Token* token = VectorGetAt(*index, tokens);
        if(!token) break;

        NodeType op;

        if(token->type == TOK_PLUS) op = NODE_ADD;
        else if(token->type == TOK_MINUS) op = NODE_SUB;
        else break;

        (*index)++;

        ASTNode* right = ParseTerm(index, tokens);
        if(!right) return NULL;

        ASTNode* node = malloc(sizeof(ASTNode));
        node->childCount = 2;
        node->children = malloc(node->childCount * sizeof(ASTNode*));
        node->type = op;
        node->children[0] = left;
        node->children[1] = right;

        left = node;
    }

    return left;
}

ASTNode* ParseTerm(size_t* index, Vector* tokens) {
    ASTNode* left = ParseFactor(index, tokens);
    if(!left) return NULL;

    while(1) {
        Token* token = VectorGetAt(*index, tokens);
        if(!token) break;

        NodeType op;

        if(token->type == TOK_STAR) op = NODE_MUL;
        else if(token->type == TOK_SLASH) op = NODE_DIV;
        else break;

        (*index)++;

        ASTNode* right = ParseFactor(index, tokens);
        if(!right) return NULL;

        ASTNode* node = malloc(sizeof(ASTNode));
        node->childCount = 2;
        node->children = malloc(node->childCount * sizeof(ASTNode*));
        node->type = op;
        node->children[0] = left;
        node->children[1] = right;

        left = node;
    }

    return left;
}

ASTNode* ParseFactor(size_t* index, Vector* tokens) {
    Token* token = VectorGetAt(*index, tokens);

    if(!token) return NULL;

    if(token->type == TOK_OPAREN) {
        (*index)++;
        ASTNode* expr = ParseExpression(index, tokens);
        if(!expr) return NULL;

        Token* next = VectorGetAt(*index, tokens);
        if(!next || next->type != TOK_CPAREN) return NULL;

        (*index)++;
        return expr;
    } else if(token->type == TOK_MINUS || token->type == TOK_BANG || token->type == TOK_TILDE) {
        (*index)++;

        ASTNode* operand = ParseFactor(index, tokens);
        if(!operand) return NULL;

        NodeType op;

        if(token->type == TOK_MINUS) op = NODE_NEG;
        else if(token->type == TOK_TILDE) op = NODE_BITNOT;
        else if(token->type == TOK_BANG) op = NODE_NOT;
        else return NULL;

        ASTNode* node = malloc(sizeof(ASTNode));
        node->childCount = 1;
        node->type = op;
        node->children = malloc(sizeof(ASTNode*));
        node->children[0] = operand;

        return node;
    } else if(token->type == TOK_INTLIT) {
        ASTNode* node = malloc(sizeof(ASTNode));
        node->type = NODE_NUMBER;
        node->children = NULL;
        node->childCount = 0;
        node->intLit = atoi(token->value);

        (*index)++;
        return node;
    }

    return NULL;
}