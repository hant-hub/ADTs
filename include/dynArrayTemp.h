
//Delibrately not header guarded This is supposed to be able to template out multiple
//types of dynArrays
#include "common.h"
#include <string.h>

//This has to be a crime of some kind
#ifndef T
#define T int
#endif

#ifndef MINSIZE
#define MINSIZE 10
#endif

#ifndef GROWTH_FACTOR
#define GROWTH_FACTOR 1.618
#endif

#define combine(a, b) a ## b
#define dynArray(n, x) combine(n, x)
#define dynHeader(x) combine(dynArrayHeader, x)

T* dynArray(DynPush_, T) (T* a, T val);
T* dynArray(DynDel_, T) (T* a, size_t idx);
T* dynArray(DynIns_, T) (T* a, T val, size_t idx);
void dynArray(DynFree_, T) (T* a);
size_t dynArray(DynSize_, T) (T* a);
size_t dynArray(DynCap_, T) (T* a);

#ifdef ADT_IMPL

typedef struct {
    size_t size;
    size_t cap;
} dynHeader(T);

#define header(a) (((dynHeader(T)*)a) - 1)

T* dynArray(DynPush_, T) (T* a, T val) {
    if (a == NULL) {
        dynHeader(T)* raw = adt_malloc(sizeof(dynHeader(T)) + sizeof(T) * MINSIZE);
        raw->size = 0;
        raw->cap = MINSIZE;
        a =(T*)(++raw);
    } else if (header(a)->size + 1 > header(a)->cap) {
        dynHeader(T)* raw = header(a);
        raw->cap *= GROWTH_FACTOR;
        raw = adt_realloc(raw, sizeof(dynHeader(T)) + header(a)->cap * sizeof(T));
        a =(T*)(++raw);
    }
    a[header(a)->size++] = val;
    return a;
}

T* dynArray(DynDel_, T) (T* a, size_t idx) {
    memmove(&a[idx], &a[idx + 1], sizeof(T) * (header(a)->size-- - idx));
    return a;
}

T* dynArray(DynIns_, T) (T* a, T val, size_t idx) {
    if (header(a)->size + 1 > header(a)->cap) {
        dynHeader(T)* raw = header(a);
        raw->cap *= GROWTH_FACTOR;
        raw = adt_realloc(raw, sizeof(dynHeader(T)) + header(a)->cap * sizeof(T));
        a =(T*)(++raw);
    }
    memmove(&a[idx + 1], &a[idx], sizeof(T) * (header(a)->size++ + idx));
    a[idx] = val;
    return a;
}

void dynArray(DynFree_, T) (T* a) {
    adt_free(header(a));
}


size_t dynArray(DynSize_, T) (T* a) {
    return header(a)->size;
}

size_t dynArray(DynCap_, T) (T* a) {
    return header(a)->cap;
}


#endif

#undef T
#undef header
#undef combine
#undef dynArray
#undef dynHeader
#undef MINSIZE
#undef GROWTH_FACTOR
