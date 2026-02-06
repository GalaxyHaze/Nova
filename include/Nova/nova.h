#ifndef NOVA_NOVA_H
#define NOVA_NOVA_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---- Memory / Arena (C ABI) ----
typedef struct NovaArena NovaArena;

NovaArena* nova_arena_create(size_t initial_block_size);
void* nova_arena_alloc(NovaArena* arena, size_t size);
char* nova_arena_strdup(NovaArena* arena, const char* str);
void nova_arena_reset(NovaArena* arena);
void nova_arena_clean_block(NovaArena* arena);
void nova_arena_destroy(NovaArena* arena);

// ---- File helpers (C ABI) ----
bool nova_file_exists(const char* path);
bool nova_file_is_regular(const char* path);
size_t nova_file_size(const char* path);
bool nova_extension_matches(const char* path, const char* expected_ext);
char* nova_load_file_to_arena(NovaArena* arena, const char* path, size_t* out_size);

// ---- Vector (C ABI) ----
typedef struct {
    void* data;
    size_t len;
    size_t cap;
    size_t elem_size;
    NovaArena* arena;
} NovaVector;

void nova_vector_init(NovaVector* vec, size_t elem_size, NovaArena* arena);
void nova_vector_push(NovaVector* vec, const void* elem);
void* nova_vector_at(const NovaVector* vec, size_t index);

// ---- List (C ABI) ----
typedef struct NovaListNode {
    void* data;
    struct NovaListNode* next;
} NovaListNode;

typedef struct {
    NovaListNode* head;
    NovaListNode* tail;
    size_t len;
    size_t elem_size;
    NovaArena* arena;
} NovaList;

void nova_list_init(NovaList* list, size_t elem_size, NovaArena* arena);
void nova_list_push_back(NovaList* list, const void* data);
void* nova_list_at(const NovaList* list, size_t index);

// ---- Tokens / Parsing (C ABI) ----
typedef struct {
    size_t index;
    size_t line;
} NovaInfo;

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

typedef struct {
    const char* value;
    size_t value_len;
    NovaInfo info;
    NovaTokenType token;
} NovaToken;

#define NOVA_NEWLINE(info_ptr) \
    do { (info_ptr)->line += 1; (info_ptr)->index = 1; } while(0)

#define NOVA_NEWLINE_SIZE(info_ptr, lines) \
    do { (info_ptr)->line += (lines); (info_ptr)->index = 1; } while(0)

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

typedef struct {
    const NovaToken* data;
    size_t len;
} NovaTokenSlice;

typedef struct NovaSlice {
    void* data;
    size_t len;
} NovaSlice;

NovaTokenType nova_lookup_keyword(const char* str, size_t len);
static inline NovaTokenType nova_lookup_keyword_cstr(const char* str) {
    return str ? nova_lookup_keyword(str, strlen(str)) : NOVA_TOKEN_IDENTIFIER;
}

NovaTokenSlice nova_tokenize(NovaArena* arena, const char* source, size_t source_len);

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

// ---- CLI (C ABI) ----
typedef enum {
    BUILD_MODE_DEBUG,
    BUILD_MODE_DEV,
    BUILD_MODE_RELEASE,
    BUILD_MODE_FAST,
    BUILD_MODE_TEST
} BuildMode;

typedef enum {
    ERROR_NONE = 0,
    ERROR_MISSING_INPUT_FILE,
    ERROR_INVALID_INPUT_FILE,
    ERROR_TOO_MANY_INPUT_FILES,
    ERROR_INVALID_BUILD_MODE,
    ERROR_CONFLICTING_OPTIONS,
    ERROR_HELP_REQUESTED,
    ERROR_VERSION_REQUESTED,
    ERROR_OUT_OF_MEMORY,
    ERROR_INTERNAL_ERROR
} Errors;

typedef struct {
    BuildMode mode;
    bool show_version;
    char iFile[260];
} Options;

typedef struct {
    Options options;
    Errors error;
} Result;

BuildMode string_to_build_mode(const NovaSlice* mode_str);

static inline bool nova_result_ok(const Result* r) {
    return r && r->error == ERROR_NONE;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NOVA_NOVA_H
