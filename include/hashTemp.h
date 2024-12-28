#include "common.h"
//{K, V} -> {Key, Val}
#ifndef V
#define V int
#endif

#ifndef K
#define K int
#endif

#ifndef MINSIZE
#define MINSIZE 10
#endif

#ifndef GROWTH_FACTOR
#define GROWTH_FACTOR 1.618
#endif

#define combine(a, b, c) a ## b ## c
#define Hashfunc(n, x, w) combine(n, x, w)
#define HashMeta(x, y) combine(dynArrayHeader, x, y)
#define HashType(x, y) combine(HashEntry_, x, y)


typedef struct {
    K key;
    V val;
} HashType(K, V);


HashType(K, V)* Hashfunc(HashIns_, K, V)(HashType(K, V)* h, K key, V val);

#define ADT_IMPL
#ifdef ADT_IMPL

typedef enum : char {
    HASH_EMPTY = 0,
    HASH_OCCUPIED,
    HASH_TOMBSTONE
} HashState;

typedef struct {
    size_t size;
    size_t cap;
    HashState* meta;
} HashMeta(K, V);

#define header(h) (((HashMeta(K,V)*)h)-1)
#define body(h) (((HashMeta(K,V)*)h)+1)

static inline size_t fnv1a_hash(void* buf, size_t len) {
    size_t hash = 14695981039346656037UL;
    char* bytes = buf;
    for (size_t i = 0; i < len; i++) {
        hash ^= bytes[i];
        hash *= 1099511628211UL;
    }
    return hash;
}

//quadratic probing
static inline size_t getIndex(HashType(K, V)* h, K key) {
    size_t hash = fnv1a_hash(&key, sizeof(K));
    size_t idx = hash % header(h)->cap;
    size_t step = 1;

    for (int i = 0; i < header(h)->size; i++) {
         if (header(h)->meta[idx] == HASH_EMPTY) { 
             header(h)->meta[idx] = HASH_OCCUPIED;
             return idx; 
         }
         idx += step;
         step++;
    }
    return 0;
}

HashType(K, V)* Hashfunc(HashIns_, K, V)(HashType(K, V)* h, K key, V val) {
    if (h == NULL) {
        h = (HashType(K, V)*)body(
                adt_malloc(sizeof(HashMeta(K, V)) + MINSIZE * sizeof(HashType(K,V)))
                );
        *header(h) = (HashMeta(K, V)){
            .size = 0,
            .cap = MINSIZE,
            .meta = adt_calloc(1, MINSIZE * sizeof(HashState))
        };
    } else if (header(h)->size + 1 > header(h)->cap) {
    }
    h[getIndex(h, key)] = (HashType(K, V)){key, val};
    header(h)->size++;
    return h;
}


#endif

#undef V
#undef K
#undef MINSIZE
#undef GROWTH_FACTOR
#undef combine
#undef Hashfunc
#undef HashMeta
