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
#define HashType(x, y) combine(HashEntry_, K, V)

typedef struct {
    K key;
    V val;
} HashType(K, V);


void Hashfunc(HashIns_, K, V)();

#define ADT_IMPL
#ifdef ADT_IMPL

typedef enum : char {
    HASH_EMPTY,
    HASH_OCCUPIED,
    HASH_TOMBSTONE
} HashState;

typedef struct {
    size_t size;
    size_t cap;
} HashMeta(K, V);


void Hashfunc(HashIns_, K, V)() {
    
}


#endif

#undef V
#undef K
#undef MINSIZE
#undef GROWTH_FACTOR
#undef combine
#undef Hashfunc
#undef HashMeta
