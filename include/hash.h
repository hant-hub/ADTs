#ifndef HASH_H
#define HASH_H

#include "common.h"
#include <stddef.h>

#define GROWTH_FACTOR 1.5

typedef enum {
    EMP,
    DEL,
    OCC
} hash_state;

typedef struct {
    size_t size;
    size_t capacity;
    hash_state* hashes;
} Hash_Header;

//Thanks to www.isthe.com for the FNV hash function
//This uses the 64 bit offset and prime
//This is the 1a version
static inline size_t fnv1a_hash(void* buf, size_t len) {
    size_t hash = 14695981039346656037UL;
    char* bytes = buf;
    for (size_t i = 0; i < len; i++) {
        hash ^= bytes[i];
        hash *= 1099511628211UL;
    }
    return hash;
}

#define header(h) (((Hash_Header*)h)-1)
#define body(h) (((Hash_Header*)h)+1)
#define hash_cap(h) header(h)->capacity
#define hash_size(h) header(h)->size
#define hash_hashes(h) header(h)->hashes
#define hash(b, l) fnv1a_hash(b, l)

#define hash_ins(h, k, v) \
    ( \
      h = h ? hash_grow(h, sizeof(h[0]), hash_size(h) + 1) : hash_grow(h, sizeof(h[0]), 1), \
      hash_hashes(h)[hash(&k, sizeof(k) % hash_cap(h)] = hash(&k, sizeof(k)), \
      h[hash(&k, sizeof(k)) % hash_cap(h)].key = k, \
      h[hash(&k, sizeof(k)) % hash_cap(h)].val = v \
    )

#define hash_get(h, k) \
    ( \
      h[hash(&k, sizeof(k)) % hash_cap(h)].val \
    )


static inline ptrdiff_t hash_get_index(void* h, void* key, size_t keyLength) {
    return -1;
}


static inline void* hash_grow(void* h, size_t elemSize, size_t min) {

    if (h == NULL) {
        if (min < 50) min = 50;
        h = body(adt_malloc(sizeof(Hash_Header) + min * elemSize));
        *header(h) = (Hash_Header){
            .size = 0,
            .capacity = min,
            .hashes = adt_malloc(sizeof(hash_state) * min) //one hash per entry
        };
    } else if (min > hash_cap(h)) {
        printf("Resize not implemented\n");
    }

    return h;
}

#undef GROWTH_FACTOR


#endif
