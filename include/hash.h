#ifndef HASH_H
#define HASH_H

#include "common.h"
#include <stddef.h>
#include <string.h>

#define GROWTH_FACTOR 1.5
#define SIZE_LIMIT 0.8

typedef enum {
    HASH_EMP,
    HASH_DEL,
    HASH_OCC
} hash_state;

typedef struct {
    size_t size;
    size_t tomb;
    size_t capacity;
    hash_state* meta;
    size_t temp;
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
#define hash_temp(h) header(h)->temp
#define hash_meta(h) header(h)->meta
#define hash_tomb(h) header(h)->tomb
//#define hash_hashes(h) header(h)->hashes
#define hash(b, l) fnv1a_hash(b, l)

#define hash_ins(h, k, v) \
    ( \
      h = h ? hash_grow(h, sizeof(h[0]), hash_size(h) + 1) : hash_grow(h, sizeof(h[0]), 1), \
      hash_temp(h) = hash_set_index(h, &k, sizeof(h[0]), sizeof(k)), \
      h[hash_temp(h)].key = k, \
      h[hash_temp(h)].val = v, \
      hash_size(h)++ \
    )

#define hash_get(h, k) \
    ( \
      h[hash_get_index(h, &k, sizeof(h[0]), sizeof(k))] \
    )

#define hash_geti(h, k) \
    ( \
      hash_get_index(h, &k, sizeof(h[0]), sizeof(k)) \
    )
#define hash_del(h, k) \
    (hash_tomb(h)++, hash_unset_index(h, &k, sizeof(h[0]), sizeof(k)))

    


static inline const char* hash_get_meta(void* h, size_t index) {
    static const char* states[] = {
        "Empty",
        "Deleted",
        "Occupied"
    };

    switch (hash_meta(h)[index]) {
        case HASH_EMP:
            return states[0];
        case HASH_DEL:
            return states[1];
        case HASH_OCC:
            return states[2];
    }
    return NULL;
}

static inline ptrdiff_t hash_set_index(void* h, void* key, size_t stride, size_t keyLength) {
    size_t index = hash(key, keyLength) % hash_cap(h);
    size_t step = 1;
    char* buf = h;

    hash_state* meta = hash_meta(h);
    for (int i = 0; i < hash_cap(h); i++) {
        if (meta[index] == HASH_EMP || meta[index] == HASH_DEL) {
            meta[index] = HASH_OCC;
            return index;
        }
        if (memcmp(&buf[index * stride], key, keyLength) == 0) {
            meta[index] = HASH_OCC;
            return index;
        }
        index += step;
        step++;
    }
    return -1;
}

static inline void hash_unset_index(void* h, void* key, size_t stride, size_t keyLength) {
    size_t index = hash(key, keyLength) % hash_cap(h);
    size_t step = 1;
    char* buf = h;

    hash_state* meta = hash_meta(h);
    while (1) {
        if (meta[index] == HASH_EMP) {
            return;
        }
        if (memcmp(&buf[index * stride], key, keyLength) == 0) {
            meta[index] = HASH_DEL;
            return;
        }
        index += step;
        step++;
    }
}

static inline ptrdiff_t hash_get_index(void* h, void* key, size_t stride, size_t keyLength) {
    size_t index = hash(key, keyLength) % hash_cap(h);
    size_t step = 1;
    char* buf = h;

    hash_state* meta = hash_meta(h);
    while (1) {
        if (meta[index] == HASH_EMP) {
            return -1;
        }
        if (memcmp(&buf[index * stride], key, keyLength) == 0) {
            return index;
        }
        index += step;
        step++;
    }
    return -1;
}

static inline int hash_test(void* h) {
    if (hash_size(h) > hash_cap(h) * SIZE_LIMIT) { 
        return 1;
    }
    return 0;
}

static inline void* hash_grow(void* h, size_t elemSize, size_t min) {

    if (h == NULL) {
        if (min < 50) min = 50;
        h = body(adt_malloc(sizeof(Hash_Header) + min * elemSize));
        *header(h) = (Hash_Header){
            .size = 0,
            .capacity = min,
            .meta = adt_malloc(sizeof(hash_state) * min) //one hash per entry
        };
    } else if (min > hash_cap(h)) {
        printf("Resize not implemented\n");
    }

    return h;
}

static inline void* hash_rebuild(void* h, size_t stride, size_t keyLength) {
    return h;
}

#undef GROWTH_FACTOR


#endif
