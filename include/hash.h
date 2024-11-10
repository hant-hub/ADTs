#ifndef HASH_H
#define HASH_H

#include "common.h"
#include <stddef.h>


typedef struct {
    size_t size;
    size_t capacity;
    size_t* hashes;
    ptrdiff_t* indicies;
    int temp;
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
    (h)[hash(k) % hash_cap(h)] = v, \
    hash_hashes(h)[hash(k) % hash_cap(h)] = hash(k), \
    hash_size(h)++\
    )


//assume the key hash is in h
static inline void hash_set_index(void* h) {
    size_t key = hash_temp(h) % hash_cap(h); 
    

}

static inline void* hash_grow(void* h, size_t size, size_t bucketSize) {
    if (h == NULL) {
        if (size < 50) size = 50; 
        size_t* hashes = adt_malloc(size * sizeof(size_t));
        size_t* indicies = adt_malloc(size * sizeof(size_t));
        h = body(adt_malloc(sizeof(Hash_Header) + size * bucketSize));
        *header(h) = (Hash_Header) {
            .size = 0,
            .capacity = size,
            .hashes = hashes
        };
    } else {
        printf("Resize not implemented\n");
    }
    return h;
}




#endif
