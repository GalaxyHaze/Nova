// include/Nova/parse/tokens.h
#ifndef NOVA_PARSE_TOKENS_H
#define NOVA_PARSE_TOKENS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// --- Info ---
typedef struct {
    size_t index;  // 0-based column/offset
    size_t line;   // 1-based line number
} NovaInfo;

// --- TokenType ---
// Enum values are preserved exactly as in your C++ version
typedef enum {
    NOVA_TOKEN_STRING,
    NOVA_TOKEN_NUMBER,
    NOVA_TOKEN_TYPE,
    NOVA_TOKEN_IDENTIFIER,
    NOVA_TOKEN_MODIFIER,

    NOVA_TOKEN_ASSIGNMENT,
    NOVA_TOKEN_EQUAL,
    NOVA_TOKEN_NOT_EQUAL,
    NOVA_TOKEN_PLUS,
    NOVA_TOKEN_MINUS,
    NOVA_TOKEN_MULTIPLY,
    NOVA_TOKEN_DIVIDE,

    NOVA_TOKEN_CONST,
    NOVA_TOKEN_LET,
    NOVA_TOKEN_AUTO,
    NOVA_TOKEN_MUTABLE,

    NOVA_TOKEN_GREATER_THAN,
    NOVA_TOKEN_LESS_THAN,
    NOVA_TOKEN_GREATER_THAN_OR_EQUAL,
    NOVA_TOKEN_LESS_THAN_OR_EQUAL,

    NOVA_TOKEN_AND,
    NOVA_TOKEN_OR,

    NOVA_TOKEN_LPAREN,
    NOVA_TOKEN_RPAREN,
    NOVA_TOKEN_LBRACE,
    NOVA_TOKEN_RBRACE,
    NOVA_TOKEN_LBRACKET,
    NOVA_TOKEN_RBRACKET,

    NOVA_TOKEN_COMMA,
    NOVA_TOKEN_COLON,
    NOVA_TOKEN_SEMICOLON,

    NOVA_TOKEN_UNKNOWN,

    NOVA_TOKEN_RETURN,
    NOVA_TOKEN_END,
    NOVA_TOKEN_IF,
    NOVA_TOKEN_ELSE,
    NOVA_TOKEN_WHILE,
    NOVA_TOKEN_FOR,
    NOVA_TOKEN_IN,
    NOVA_TOKEN_ARROW,
    NOVA_TOKEN_PLUS_EQUAL,
    NOVA_TOKEN_MINUS_EQUAL,
    NOVA_TOKEN_MULTIPLY_EQUAL,
    NOVA_TOKEN_DIVIDE_EQUAL,
    NOVA_TOKEN_DOT,
    NOVA_TOKEN_DOTS,
    NOVA_TOKEN_SWITCH,
    NOVA_TOKEN_STRUCT,
    NOVA_TOKEN_ENUM,
    NOVA_TOKEN_UNION,
    NOVA_TOKEN_FAMILY,
    NOVA_TOKEN_BREAK,
    NOVA_TOKEN_CONTINUE,
    NOVA_TOKEN_MOD,
    NOVA_TOKEN_ENTITY,
    NOVA_TOKEN_FLOAT,
    NOVA_TOKEN_NOT,
    NOVA_TOKEN_HEXADECIMAL,
    NOVA_TOKEN_OCTONAL,
    NOVA_TOKEN_BINARY
} NovaTokenType;

// --- Token ---
typedef struct {
    const char* value;        // points into source buffer (Arena-owned)
    size_t value_len;         // length of value (since no null-termination guaranteed)
    NovaInfo info;
    NovaTokenType token;
} NovaToken;

    inline void newLine(NovaInfo* self) {
        self->line += 1;
        self->index = 1;
    }
    inline void newLineSize(NovaInfo* self, const size_t line) {
    self->line += line;
    self->index = 1;
}

// --- Instruction (IR) ---
typedef enum {
    NOVA_INST_ADD,
    NOVA_INST_SUB,
    NOVA_INST_MUL,
    NOVA_INST_DIV,
    NOVA_INST_POINT_TO,
    NOVA_INST_ACCESS_IN,
    NOVA_INST_EMPLACE,
    NOVA_INST_STORE
    // extend as needed
} NovaInstruction;

// --- Type System ---
typedef enum {
    NOVA_TYPE_U8, NOVA_TYPE_U16, NOVA_TYPE_U32, NOVA_TYPE_U64, NOVA_TYPE_U128, NOVA_TYPE_U256,
    NOVA_TYPE_I8, NOVA_TYPE_I16, NOVA_TYPE_I32, NOVA_TYPE_I64, NOVA_TYPE_I128, NOVA_TYPE_I256,
    NOVA_TYPE_F8, NOVA_TYPE_F16, NOVA_TYPE_F32, NOVA_TYPE_F64, NOVA_TYPE_F128, NOVA_TYPE_F256,
    NOVA_TYPE_BOOLEAN,
    NOVA_TYPE_CHARACTER,
    NOVA_TYPE_BYTE,
    NOVA_TYPE_NULL,
    NOVA_TYPE_POINTER
} NovaType;

#ifdef __cplusplus
}
#endif

#endif // NOVA_PARSE_TOKENS_H