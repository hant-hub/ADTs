#ifndef HASH_H
#define HASH_H

#include "common.h"
#include <stddef.h>
#include <string.h>

#define GROWTH_FACTOR 1.5

typedef struct {
    size_t size;
    size_t capacity;
    size_t* hashes;
    ptrdiff_t* indicies;
    ptrdiff_t temp;
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

#define hash(h) fnv1a_hash(&h, element_size(h))
#define header(h) (((Hash_Header*)h)-1)
#define body(h) (void*)(((Hash_Header*)h)+1)
#define hash_size(h) header(h)->size
#define hash_cap(h) header(h)->capacity
#define hash_hashes(h) header(h)->hashes
#define hash_indicies(h) header(h)->indicies
#define hash_temp(h) header(h)->temp

    
#define hash_put(h, k, v) \
   ((h = h ? hash_grow(h, hash_size(h) + 1, element_size(h)) : hash_grow(h, 1, element_size(h))),   \
    hash_temp(h) = hash(k), \
    h[hash_set_index(h)] = v \
    )

#define hash_get(h,k) \
    (hash_temp(h) = hash(k), hash_temp(h) = hash_get_index(h), h[hash_temp(h)])


static inline ptrdiff_t hash_get_index(void* h) {
    size_t key = hash_temp(h) % hash_cap(h); 
    size_t step = 1;
    
    ptrdiff_t* indicies = hash_indicies(h); 
    size_t* hashes = hash_hashes(h);
    int empty = 0;

    while (!empty) {
        if (indicies[key] == -1) {
            break;
        }
        if (hashes[key] == hash_temp(h)) {
            return indicies[key];
        }
        key = (key + step) % hash_cap(h);
        step += 1;
    }
    return -1;
}

//assume the key hash is in h
static inline ptrdiff_t hash_set_index(void* h) {
    size_t key = hash_temp(h) % hash_cap(h); 
    size_t step = 1;
    
    ptrdiff_t* indicies = hash_indicies(h); 
    ptrdiff_t index = -1;
    while (index == -1) {
        if (indicies[key] == -1) index = key;
        key = (key + step) % hash_cap(h);
        step += 1;
    }
    indicies[index] = hash_size(h);
    hash_hashes(h)[index] = hash_temp(h);
    return hash_size(h)++;
}

static inline void* hash_grow(void* h, size_t size, size_t bucketSize) {
    if (h == NULL) {
        if (size < 50) size = 50; 
        size_t* hashes = adt_malloc(size * sizeof(size_t));
        ptrdiff_t* indicies = adt_malloc(size * sizeof(ptrdiff_t));
        
        memset(hashes, 0, size * sizeof(size_t));
        memset(indicies, -1, size * sizeof(size_t));

        h = body(adt_malloc(sizeof(Hash_Header) + size * bucketSize));
        *header(h) = (Hash_Header) {
            .size = 0,
            .capacity = size,
            .hashes = hashes,
            .indicies = indicies,
        };
    } else if (size > hash_cap(h)) {
        printf("Resize not implemented\n");
    }
    return h;
}

static inline void hash_destroy(void* h) {
    free(hash_hashes(h));
    free(hash_indicies(h));
    free(header(h));
}


#undef GROWTH_FACTOR


#endif
