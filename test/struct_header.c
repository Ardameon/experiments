#include <stdio.h>
#include <stdlib.h>

typedef struct header_t
{
    int size;
    void *p[0];
} header_t;

typedef struct header2_t
{
    int size;
    void *p[];
} header2_t;

int main()
{
    header_t *obj = malloc(sizeof(header_t) + sizeof(int));
    header2_t *obj2 = malloc(sizeof(header2_t) + sizeof(int));

    int *val = (int *)obj->p;
    *val = 1;
    printf("start: %p\n"
           "size: %p\n"
           "p: %p\n"
           "p_val: %p\n"
           "val: %p\n"
           "*val: %d\n",
            obj, &obj->size, &obj->p, obj->p, val, *val);

    int *val2 = (int *)obj2->p;
    *val2 = 2;

    printf("\nstart: %p\n"
           "size: %p\n"
           "p: %p\n"
           "p_val2: %p\n"
           "val2: %p\n"
           "*val2: %d\n",
            obj2, &obj2->size, &obj2->p, obj2->p, val2, *val2);

    return 0;
}
