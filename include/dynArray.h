#ifndef ADT_DYNARRAY_H
#define ADT_DYNARRAY_H

#include "common.h"
#include <stddef.h>

#define GROWTH_FACTOR 1.5

typedef struct {
    size_t capacity;
    size_t size;
} dynArray_Header;

#define adt_dynarray_header(a) (((dynArray_Header*)a)-1)
#define body(a) ((void*)(((dynArray_Header*)a)+1))

#define arr_len(a) adt_dynarray_header(a)->size
#define arr_cap(a) adt_dynarray_header(a)->capacity
#define arr_push(a, v) (a ? dyn_array_grow((void**)(&a), arr_len(a) + 1, element_size(a)) : dyn_array_grow((void**)(&a),1, element_size(a)), (a[arr_len(a)++] = v))
#define arr_pop(a) (a) ? a[--arr_len(a)] : 0
#define arr_set(a, l) ((((a == NULL) || (arr_cap(a) < l)) ? dyn_array_grow((void**)(&a), l, element_size(a)) : 0), arr_len(a) = l) 
#define arr_ins(a, i, v) \
    ( \
    dyn_array_grow((void**)(&a), arr_len(a) + 1, element_size(a[0])), \
    dyn_array_shift(&a[i], element_size(a[0]) * (arr_len(a)-i), 1 * (int)element_size(a[0])), \
    arr_len(a)++, \
    a[i] = v \
    )

#define arr_del(a, i) \
    ( \
    dyn_array_shift(&a[i], element_size(a[0]) * (arr_len(a)-i), -1 * (int)element_size(a[0])), \
    arr_len(a)-- \
    )


static inline void dyn_array_shift(void* start, ptrdiff_t end, int offset) {
    char* buf = start;
    if (offset > 0) {
        for (ptrdiff_t i = end; i >= 0; i--) {
            buf[i] = buf[i - offset]; 
        }
    } else if (offset < 0) {
        offset *= -1;
        for (size_t i = 0; i < end; i++) {
            buf[i] = buf[i + offset]; 
        }
    }
}

static inline int dyn_array_grow(void **ref, int val, size_t elemSize) {
    void* a = *ref;
    if (a == NULL) {
        int size = val < 4 ? 4 : val;
        a = body(adt_malloc(sizeof(dynArray_Header) + size * elemSize));
        *(adt_dynarray_header(a)) = (dynArray_Header){
           .capacity = size,
           .size = 0,
        };
    } else if (val > adt_dynarray_header(a)->capacity) {
        while (adt_dynarray_header(a)->capacity < val) adt_dynarray_header(a)->capacity *= GROWTH_FACTOR;
        a = body(adt_realloc(adt_dynarray_header(a), sizeof(dynArray_Header) + adt_dynarray_header(a)->capacity * elemSize));
    }
    *ref = a;
    return 0;
}

static inline void adt_destroy(void* a) {
    adt_free(adt_dynarray_header(a));
}

#undef body
#undef GROWTH_FACTOR




#endif
