// include/Nova/ast/ast.h
#ifndef NOVA_AST_AST_H
#define NOVA_AST_AST_H

#include "Nova/utils/slice.h"

#ifdef __cplusplus
extern "C" {
#endif

    struct NovaArena;

    typedef enum {
        NOVA_NODE_END = 0x00,
        NOVA_NODE_LITERAL = 0x01,
        NOVA_NODE_BINARY_EXPRESSION = 0x02,
        NOVA_NODE_IDENTIFIER = 0x03,
        NOVA_NODE_VARIABLE_DECLARATION = 0x04,
        NOVA_NODE_FUNCTION_DECLARATION = 0x05,
        NOVA_NODE_IF_STATEMENT = 0x06,
        NOVA_NODE_WHILE_STATEMENT = 0x07,
        NOVA_NODE_RETURN_STATEMENT = 0x08,
        NOVA_NODE_UNARY_EXPRESSION = 0x09,
        NOVA_NODE_ASSIGNMENT = 0x0A,
        NOVA_NODE_BLOCK = 0x0B,
        NOVA_NODE_CALL_EXPRESSION = 0x0C,
        NOVA_NODE_MEMBER_ACCESS = 0x0D,
        NOVA_NODE_INDEX_EXPRESSION = 0x0E,
        NOVA_NODE_FOR_STATEMENT = 0x0F,
        NOVA_NODE_SWITCH_STATEMENT = 0x10,
        NOVA_NODE_CASE_STATEMENT = 0x11,
        NOVA_NODE_BREAK_STATEMENT = 0x12,
        NOVA_NODE_CONTINUE_STATEMENT = 0x13,
        NOVA_NODE_STRUCT_DECLARATION = 0x14,
        NOVA_NODE_ENUM_DECLARATION = 0x15,
        NOVA_NODE_UNION_DECLARATION = 0x16,
        NOVA_NODE_UNKNOWN = 0xFF
    } NovaNodeType;

    typedef struct ASTNode ASTNode;
    typedef ASTNode* NovaNode;

    NovaNode nova_parse(NovaTokenSlice tok, NovaArena* arena);
    NovaNodeType nova_node_get_type(NovaNode node);

#ifdef __cplusplus
}
#endif

#endif // NOVA_AST_AST_H
