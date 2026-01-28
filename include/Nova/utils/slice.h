// include/Nova/utils/slice.h
#ifndef NOVA_UTILS_SLICE_H
#define NOVA_UTILS_SLICE_H

#include "../parse/tokens.h"

#ifdef __cplusplus
extern "C" {
#endif

    // Generic slice: [data, data + len)
    typedef struct {
        void* data;
        size_t len;
        size_t elem_size; // for generic use
    } NovaSlice;

    // Typed helpers (optional macros)
#define NOVA_SLICE(T, ptr, n) ((NovaSlice){ .data = (void*)(ptr), .len = (n), .elem_size = sizeof(T) })

    // For tokens specifically
    typedef struct {
        const NovaToken* data;
        size_t len;
    } NovaTokenSlice;

#ifdef __cplusplus
}
#endif

#endif // NOVA_UTILS_SLICE_H