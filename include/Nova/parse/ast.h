// include/Nova/ast/ast.h
#ifndef NOVA_AST_AST_H
#define NOVA_AST_AST_H

#include "../utils/slice.h"

#ifdef __cplusplus
extern "C" {
#endif

    struct NovaArena;

    typedef enum {
        NOVA_NODE_END = 0x0,
        NOVA_NODE_LITERAL,
        NOVA_NODE_BINARY_EXPRESSION,
        NOVA_NODE_IDENTIFIER,
        NOVA_NODE_VARIABLE_DECLARATION,
        NOVA_NODE_FUNCTION_DECLARATION,
        NOVA_NODE_IF_STATEMENT,
        NOVA_NODE_WHILE_STATEMENT,
        NOVA_NODE_RETURN_STATEMENT,
        NOVA_NODE_UNKNOWN = 0xFF
    } NovaNodeType;

    typedef struct ASTNode ASTNode;
    typedef ASTNode* NovaNode;

    // Public functions
    NovaNode nova_parse(NovaTokenSlice tokens, struct NovaArena* arena);
    NovaNodeType nova_node_get_type(NovaNode node);

#ifdef __cplusplus
}
#endif

#endif // NOVA_AST_AST_H