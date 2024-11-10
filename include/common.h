#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>

#define element_size(a) sizeof((typeof(a))0)

#ifndef adt_malloc
#define adt_malloc(n) malloc(n)
#endif

#ifndef adt_realloc
#define adt_realloc(p, n) realloc(p, n)
#endif

#endif
