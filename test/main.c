#include <stdio.h>
#include <stdlib.h>

static int allocs = 0;

static void* deb_calloc(size_t n, size_t e, const char* file, const int line) {
    printf("Alloc of size %ld at %s:%d\n", n, file, line);
    allocs++;
    return calloc(n, e);
}

static void* deb_malloc(size_t n, const char* file, const int line) {
    printf("Alloc of size %ld at %s:%d\n", n, file, line);
    allocs++;
    return malloc(n);
}

static void* deb_realloc(void* p, size_t n, const char* file, const int line) {
    printf("Alloc of size %ld at %s:%d\n", n, file, line);
    allocs++;
    return realloc(p, n);
}

#define adt_malloc(n) deb_malloc(n, __FILE__ , __LINE__)
#define adt_realloc(p, n) deb_realloc(p, n, __FILE__, __LINE__)
#define adt_calloc(n, e) deb_calloc(n, e, __FILE__, __LINE__)
#include "dynArray.h"
#include "hash.h"

int main() {
    long* a = NULL;
    arr_set(a, 30);
    for (int i = 0; i < 30; i++) {
        a[i] = i;
    }
    arr_push(a, 31);
    arr_push(a, 33);
    for (int i = 0; i < arr_len(a); i++) {
        printf("%d: %ld\n", i, a[i]);
    }
    printf("allocs: %d\n", allocs);
    adt_destroy(a);

    
    int show_hashes = 0;
    int* h = NULL;
    char k = 'a';
    char k2 = 'b';
    hash_put(h, k, 1);
    hash_put(h, k2, 2);
    for (int i = 0; i < hash_size(h); i++) {
        printf("h: %d\n", h[i]);
    }
    printf("(k,v): (%c, %d)\n", k, hash_get(h, k));
    printf("(k,v): (%c, %d)\n", k, hash_get(h, k2));

    if (show_hashes) {
        for (int i = 0; i < hash_cap(h); i++) {
            printf("hash: 0x%.16lX\t%ld\n", hash_hashes(h)[i], hash_indicies(h)[i]);
        }
    }

    hash_destroy(h);

    return 0;
}
