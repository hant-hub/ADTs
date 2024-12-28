#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static int allocs = 0;

void* deb_calloc(size_t n, size_t e, const char* file, const int line) {
    printf("Alloc of size %ld at %s:%d\n", n * e, file, line);
    allocs++;
    return calloc(n, e);
}

void* deb_malloc(size_t n, const char* file, const int line) {
    printf("Alloc of size %ld at %s:%d\n", n, file, line);
    allocs++;
    return malloc(n);
}

void* deb_realloc(void* p, size_t n, const char* file, const int line) {
    printf("Alloc of size %ld at %s:%d\n", n, file, line);
    allocs++;
    return realloc(p, n);
}

#define adt_malloc(n) deb_malloc(n, __FILE__ , __LINE__)
#define adt_realloc(p, n) deb_realloc(p, n, __FILE__, __LINE__)
#define adt_calloc(n, e) deb_calloc(n, e, __FILE__, __LINE__)
#include "dynArray.h"
#define ADT_IMPL
#define T int
#include "dynArrayTemp.h"
#define T long
#include "dynArrayTemp.h"
#include "hashTemp.h"

int main() {
    int* a = NULL;
    arr_set(a, 30);
    for (int i = 0; i < 30; i++) {
        a[i] = i;
    }
    arr_push(a, 31);
    arr_push(a, 32);
    arr_push(a, 33);
    arr_ins(a, 11, -1);
    arr_del(a, 9);
    
    for (int i = 0; i < arr_len(a); i++) {
        printf("%d: %d\n", i, a[i]);
    }
    printf("allocs: %d\n", allocs);
    adt_destroy(a);

    
//    int show_hashes = 0;
//    char k1 = 'a';
//    char k2 = 'b';
//    char k3 = 'c';
//    struct {char key; int val;}* h = NULL;
//    struct {char key; int val;}* h2 = NULL;
//    hash_ins(h, k1, 1);
//    hash_ins(h, k2, 2);
//    hash_ins(h, k3, 3);
//
//    ptrdiff_t index1 = hash_geti(h, k1);
//    ptrdiff_t index2 = hash_geti(h, k2); 
//    ptrdiff_t index3 = hash_geti(h, k3); 
//    printf("(k, v): %ld (%c, %d) %s\n", index1, k1, h[index1].val, hash_get_meta(h, index1));
//    printf("(k, v): %ld (%c, %d) %s\n", index2, k2, h[index2].val, hash_get_meta(h, index2));
//    printf("(k, v): %ld (%c, %d) %s\n", index3, k3, h[index3].val, hash_get_meta(h, index3));
//
//    printf("Delete %c\n", k1);
//    printf("(k, v): %ld (%c, %d) %s\n", index1, k1, h[index1].val, hash_get_meta(h, index1));
//    printf("(k, v): %ld (%c, %d) %s\n", index2, k2, h[index2].val, hash_get_meta(h, index2));
//    printf("(k, v): %ld (%c, %d) %s\n", index3, k3, h[index3].val, hash_get_meta(h, index3));
//
//
//    printf("Rebuild\n");
//    h2 = hash_rebuild(h, hash_cap(h), sizeof(h[0]), sizeof(h[0].key));
//    index1 = hash_geti(h2, k1);
//    index2 = hash_geti(h2, k2); 
//    index3 = hash_geti(h2, k3); 
//    printf("(k, v): %ld (%c, %d) %s\n", index1, k1, h2[index1].val, hash_get_meta(h2, index1));
//    printf("(k, v): %ld (%c, %d) %s\n", index2, k2, h2[index2].val, hash_get_meta(h2, index2));
//    printf("(k, v): %ld (%c, %d) %s\n", index3, k3, h2[index3].val, hash_get_meta(h2, index3));
//
//    for (int i = 'A'; i <= 'z'; i++) {
//        char k = i;
//        hash_ins(h2, k, i);
//        hash_del(h2, k);
//    }
//    for (int i = 'A'; i <= 'z'; i++) {
//        char k = i;
//        ptrdiff_t index = hash_geti(h2, k);
//        printf("(%c, %d)\n", h2[index].key, h2[index].val);
//    }
//
//    hash_destroy(h2);

    int* dyntem = NULL;
    for (int i = 0; i < 20; i++) {
        dyntem = DynPush_int(dyntem, i);
    }
    for (int i = 0; i < DynSize_int(dyntem); i++) {
        printf("%d\n", dyntem[i]);
    }
    for (int i = 0; i < 20; i++) {
        DynDel_int(dyntem, 20 - i);
    }
    for (int i = 0; i < DynSize_int(dyntem); i++) {
        printf("%d\n", dyntem[i]);
    }

    DynFree_int(dyntem);

    HashEntry_intint* h = HashIns_intint(NULL, 1, 1);
    h = HashIns_intint(h, 2, 2);

    for (int i = 0; i < 10; i++) {
        printf("(k, v): (%d, %d)\n", h[i].key, h[i].val);
    }
    return 0;
}
