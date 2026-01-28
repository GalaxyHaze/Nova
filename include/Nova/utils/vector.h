// include/Nova/utils/vector.h
#ifndef NOVA_UTILS_VECTOR_H
#define NOVA_UTILS_VECTOR_H

#include <stddef.h>
#include "../memory/arena_c_functions.h"

#ifdef __cplusplus
extern "C" {
#endif

    // Simple vector that grows in Arena (no free!)
    typedef struct {
        void* data;
        size_t len;
        size_t cap;
        size_t elem_size;
        NovaArena* arena;
    } NovaVector;

    // Initialize vector
    void nova_vector_init(NovaVector* vec, size_t elem_size, NovaArena* arena);

    // Push one element (copies bytes)
    void nova_vector_push(NovaVector* vec, const void* elem);

    // Typed macro (use in C++)
#define NOVA_VECTOR_PUSH(vec, val) \
do { \
typeof(val) _tmp = (val); \
nova_vector_push((vec), &_tmp); \
} while(0)

    // Access by index
    void* nova_vector_at(const NovaVector* vec, size_t index);

#ifdef __cplusplus
}
#endif

#endif // NOVA_UTILS_VECTOR_H