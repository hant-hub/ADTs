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


static inline int* push_back(void *a, int val) {
    if (a == NULL) {
        a = body(malloc(sizeof(dynArray_Header) + 2 * sizeof(int)));
        *(header(a)) = (dynArray_Header){
           .capacity = 2,
           .size = 0,
        };
    } else if (header(a)->size + 1 > header(a)->capacity) {
        header(a)->capacity *= GROWTH_FACTOR;
        a = body(realloc(header(a), sizeof(dynArray_Header) + header(a)->capacity * sizeof(int)));
    }
    ((typeof(val)*)a)[header(a)->size++] = val;
    return a;
}

static inline void destroy(void* a) {
    free(header(a));
}



#undef malloc
#undef realloc


#endif
