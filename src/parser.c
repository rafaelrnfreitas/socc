#include "include/parser.h"

ASTNode* ParseProgram(Vector* tokens) {
    ASTNode* program = CreateNode(NODE_PROGRAM);
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
    ASTNode* function = CreateNode(NODE_FUNCTION);

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
    function->children = realloc(function->children, function->childCount * sizeof(ASTNode*));
    function->children[function->childCount - 1] = block;

    return function;
}

ASTNode* ParseBlock(size_t* index, Vector* tokens) {
    ASTNode* block = CreateNode(NODE_BLOCK);

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
    ASTNode* node = CreateNode(NODE_RETURN);
    (*index)++;

    ASTNode* expr = ParseExpression(index, tokens);
    if(!expr) return NULL;

    node->childCount = 1;
    node->children = realloc(node->children, sizeof(ASTNode*));
    node->children[0] = expr;

    Token* token = VectorGetAt(*index, tokens);
    if(token->type != TOK_SEMICOLON) return NULL;

    (*index)++;

    return node;
}

ASTNode* ParseExpression(size_t* index, Vector* tokens) {
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

        ASTNode* node = CreateNode(op);
        node->childCount = 2;
        node->children = realloc(node->children, node->childCount * sizeof(ASTNode*));
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

        ASTNode* node = CreateNode(op);
        node->childCount = 2;
        node->children = realloc(node->children, node->childCount * sizeof(ASTNode*));
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

        ASTNode* node = CreateNode(op);
        node->childCount = 1;
        node->children = realloc(node->children, sizeof(ASTNode*));
        node->children[0] = operand;

        return node;
    } else if(token->type == TOK_INTLIT) {
        ASTNode* node = CreateNode(NODE_NUMBER);
        node->intLit = atoi(token->value);

        (*index)++;
        return node;
    }

    return NULL;
}