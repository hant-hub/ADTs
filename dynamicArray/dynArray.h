#ifndef ADT_DYNARRAY_H
#define ADT_DYNARRAY_H

#include <stdio.h>
#include <stdlib.h>

#define GROWTH_FACTOR 1.5

static int allocs = 0;

static void* deb_malloc(size_t n) {
    allocs++;
    return malloc(n);
}

static void* deb_realloc(void* p, size_t n) {
    allocs++;
    return realloc(p, n);
}


#define malloc(n) deb_malloc(n)
#define realloc(p, n) deb_realloc(p, n)


//todo convert to macro?
typedef struct {
    size_t capacity;
    size_t size;
} dynArray_Header;

#define header(a) (((dynArray_Header*)a)-1)
#define body(a) ((void*)(((dynArray_Header*)a)+1))

#define arr_len(a) header(a)->size
#define arr_cap(a) header(a)->capacity
#define arr_push(a, v) a = a ? grow(a, arr_len(a) + 1) : grow(a,1); a[arr_len(a)] = v; arr_len(a)++ 
#define arr_pop(a) (a) ? a[--arr_len(a)] : 0
#define arr_set(a, l) a = (a == NULL) || (arr_cap(a) < l) ? grow(a, l) : a 



static inline int* grow(void *a, int val) {
    if (a == NULL) {
        int size = val < 4 ? 4 : val;
        a = body(malloc(sizeof(dynArray_Header) + size * sizeof(int)));
        *(header(a)) = (dynArray_Header){
           .capacity = size,
           .size = 0,
        };
    } else if (val > header(a)->capacity) {
        while (header(a)->capacity < val) header(a)->capacity *= GROWTH_FACTOR;
        a = body(realloc(header(a), sizeof(dynArray_Header) + header(a)->capacity * sizeof(int)));
    }
    return a;
}

static inline void destroy(void* a) {
    free(header(a));
}



#undef malloc
#undef realloc


#endif
