// include/Nova/parse/tokenizer.h
#ifndef NOVA_PARSE_TOKENIZER_H
#define NOVA_PARSE_TOKENIZER_H

#include <stddef.h>
#include "../utils/slice.h"    // defines NovaTokenSlice

#ifdef __cplusplus
extern "C" {
#endif

    struct NovaArena;
    NovaTokenSlice nova_tokenize(struct NovaArena* arena, const char* source, size_t source_len);

#ifdef __cplusplus
}
#endif

#endif // NOVA_PARSE_TOKENIZER_H