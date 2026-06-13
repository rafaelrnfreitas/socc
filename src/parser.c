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

    ASTNode* expr = ParseLogicalOrExpr(index, tokens);
    if(!expr) return NULL;

    node->childCount = 1;
    node->children = malloc(sizeof(ASTNode*));
    node->children[0] = expr;

    Token* token = VectorGetAt(*index, tokens);
    if(token->type != TOK_SEMICOLON) return NULL;

    (*index)++;

    return node;
}

ASTNode* ParseLogicalOrExpr(size_t* index, Vector* tokens) {
    ASTNode* left = ParseLogicalAndExpr(index, tokens);
    if(!left) return NULL;

    while(1) {
        Token* token = VectorGetAt(*index, tokens);
        if(!token) break;

        NodeType op;

        if(token->type == TOK_PIPE_PIPE) op = NODE_LOG_OR;
        else break;

        (*index)++;

        ASTNode* right = ParseLogicalAndExpr(index, tokens);
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

ASTNode* ParseLogicalAndExpr(size_t* index, Vector* tokens) {
    ASTNode* left = ParseBitwiseOrExpr(index, tokens);
    if(!left) return NULL;

    while(1) {
        Token* token = VectorGetAt(*index, tokens);
        if(!token) break;

        NodeType op;

        if(token->type == TOK_AMP_AMP) op = NODE_LOG_AND;
        else break;

        (*index)++;

        ASTNode* right = ParseBitwiseOrExpr(index, tokens);
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

ASTNode* ParseBitwiseOrExpr(size_t* index, Vector* tokens) {
    ASTNode* left = ParseBitwiseXorExpr(index, tokens);
    if(!left) return NULL;

    while(1) {
        Token* token = VectorGetAt(*index, tokens);
        if(!token) break;

        NodeType op;

        if(token->type == TOK_PIPE) op = NODE_BIT_OR;
        else break;

        (*index)++;

        ASTNode* right = ParseBitwiseXorExpr(index, tokens);
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

ASTNode* ParseBitwiseXorExpr(size_t* index, Vector* tokens) {
    ASTNode* left = ParseBitwiseAndExpr(index, tokens);
    if(!left) return NULL;

    while(1) {
        Token* token = VectorGetAt(*index, tokens);
        if(!token) break;

        NodeType op;

        if(token->type == TOK_CARET) op = NODE_BIT_XOR;
        else break;

        (*index)++;

        ASTNode* right = ParseBitwiseAndExpr(index, tokens);
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

ASTNode* ParseBitwiseAndExpr(size_t* index, Vector* tokens) {
    ASTNode* left = ParseEqualityExpr(index, tokens);
    if(!left) return NULL;

    while(1) {
        Token* token = VectorGetAt(*index, tokens);
        if(!token) break;

        NodeType op;

        if(token->type == TOK_AMP) op = NODE_BIT_AND;
        else break;

        (*index)++;

        ASTNode* right = ParseEqualityExpr(index, tokens);
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


ASTNode* ParseEqualityExpr(size_t* index, Vector* tokens) {
    ASTNode* left = ParseRelationalExpr(index, tokens);
    if(!left) return NULL;

    while(1) {
        Token* token = VectorGetAt(*index, tokens);
        if(!token) break;

        NodeType op;

        if(token->type == TOK_EQUAL_EQUAL) op = NODE_EQUAL;
        else if(token->type == TOK_BANG_EQUAL) op = NODE_NOT_EQUAL;
        else break;

        (*index)++;

        ASTNode* right = ParseRelationalExpr(index, tokens);
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

ASTNode* ParseRelationalExpr(size_t* index, Vector* tokens) {
    ASTNode* left = ParseShiftExpr(index, tokens);
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

        ASTNode* right = ParseShiftExpr(index, tokens);
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

ASTNode* ParseShiftExpr(size_t* index, Vector* tokens) {
    ASTNode* left = ParseAdditiveExpr(index, tokens);
    if(!left) return NULL;

    while(1) {
        Token* token = VectorGetAt(*index, tokens);
        if(!token) break;

        NodeType op;

        if(token->type == TOK_GREATER_GREATER) op = NODE_SHIFT_RIGHT;
        else if(token->type == TOK_LESS_LESS) op = NODE_SHIFT_LEFT;
        else break;

        (*index)++;

        ASTNode* right = ParseAdditiveExpr(index, tokens);
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

ASTNode* ParseAdditiveExpr(size_t* index, Vector* tokens) {
    ASTNode* left = ParseMultiplicativeExpr(index, tokens);
    if(!left) return NULL;

    while(1) {
        Token* token = VectorGetAt(*index, tokens);
        if(!token) break;

        NodeType op;

        if(token->type == TOK_PLUS) op = NODE_ADD;
        else if(token->type == TOK_MINUS) op = NODE_SUB;
        else break;

        (*index)++;

        ASTNode* right = ParseMultiplicativeExpr(index, tokens);
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

ASTNode* ParseMultiplicativeExpr(size_t* index, Vector* tokens) {
    ASTNode* left = ParsePrimaryExpr(index, tokens);
    if(!left) return NULL;

    while(1) {
        Token* token = VectorGetAt(*index, tokens);
        if(!token) break;

        NodeType op;

        if(token->type == TOK_STAR) op = NODE_MUL;
        else if(token->type == TOK_SLASH) op = NODE_DIV;
		else if(token->type == TOK_PERCENT) op = NODE_MOD;
        else break;

        (*index)++;

        ASTNode* right = ParsePrimaryExpr(index, tokens);
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

ASTNode* ParsePrimaryExpr(size_t* index, Vector* tokens) {
    Token* token = VectorGetAt(*index, tokens);

    if(!token) return NULL;

    if(token->type == TOK_OPAREN) {
        (*index)++;
        ASTNode* expr = ParseLogicalOrExpr(index, tokens);
        if(!expr) return NULL;

        Token* next = VectorGetAt(*index, tokens);
        if(!next || next->type != TOK_CPAREN) return NULL;

        (*index)++;
        return expr;
    } else if(token->type == TOK_MINUS || token->type == TOK_BANG || token->type == TOK_TILDE) {
        (*index)++;

        ASTNode* operand = ParseMultiplicativeExpr(index, tokens);
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
