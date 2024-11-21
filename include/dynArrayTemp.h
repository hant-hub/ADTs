
//Delibrately not header guarded This is supposed to be able to template out multiple
//types of dynArrays
#include <stdio.h>

//This has to be a crime of some kind
#define T int

#define combine(a, b) a ## b
#define dynArray(x) combine(dynArray, x)

T dynArray(T) () {
    printf("test\n");
    return 1;
}



#undef TYPE
#undef combine
#undef dynArray
