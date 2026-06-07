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
    Token* token = VectorGetAt(*index, tokens);
    if(!token) return NULL;

    if(token->type == TOK_INTLIT) {
        ASTNode* node = CreateNode(NODE_NUMBER);
        node->intLit = atoi(token->value);
        (*index)++;
        return node;
    } else if(token->type == TOK_MINUS || token->type == TOK_BANG || token->type == TOK_TILDE) {
        (*index)++;

        ASTNode* node;
        if(token->type == TOK_MINUS) node = CreateNode(NODE_NEG);
        else if(token->type == TOK_BANG) node = CreateNode(NODE_NOT);
        else node = CreateNode(NODE_BITNOT);

        node->childCount = 1;
        node->children = malloc(sizeof(ASTNode*));
        node->children[0] = ParseExpression(index, tokens);

        if(!node->children[0]) return NULL;

        return node;
    }

    return NULL;
}