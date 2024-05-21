#ifndef __ARRAY__
#define __ARRAY__

typedef struct array_t array_t;

typedef int (*array_size_func)(const array_t *array);

typedef struct array_t
{
    array_size_func size;
} array_t;

int  array_size(const array_t *array);
void array_fill(array_t *array);
void array_sort(array_t *array);
int  array_find(const array_t *array, int elem);
void array_show(const array_t *array);

#endif /* __ARRAY__ */
