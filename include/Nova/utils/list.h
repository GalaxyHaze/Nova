// include/Nova/utils/list.h
#ifndef NOVA_UTILS_LIST_H
#define NOVA_UTILS_LIST_H

#include "../memory/arena_c_functions.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif // NOVA_UTILS_LIST_H