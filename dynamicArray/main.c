#include <stdio.h>
#include "dynArray.h"

int main() {

    int* a = NULL;
    arr_set(a, 3);

    arr_push(a, 1);
    arr_push(a, 2);
    arr_push(a, 3);
    printf("popped %d\n", arr_pop(a));
    arr_push(a, 4);
    arr_push(a, 5);



    for (int i = 0; i < arr_len(a); i++) {
        printf("%d: %d\n", i, a[i]);
    }
    printf("allocs: %d\n", allocs);

    destroy(a);

    return 0;
}
