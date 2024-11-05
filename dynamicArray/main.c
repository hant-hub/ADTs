#include <stdio.h>
#include "dynArray.h"

int main() {

    int* a = NULL;

    a = push_back(a, 1);
    a = push_back(a, 2);
    a = push_back(a, 3);
    a = push_back(a, 4);
    a = push_back(a, 5);
    a = push_back(a, 6);
    a = push_back(a, 7);
    a = push_back(a, 8);
    a = push_back(a, 9);

    printf("%d\n", a[0]);
    printf("%d\n", a[1]);
    printf("%d\n", a[2]);
    printf("%d\n", a[3]);
    printf("%d\n", a[4]);
    printf("%d\n", a[5]);
    printf("%d\n", a[6]);
    printf("%d\n", a[7]);
    printf("%d\n", a[8]);
    printf("allocs: %d\n", allocs);

    destroy(a);

    return 0;
}
