#include <stdio.h>

typedef struct {
    char str[20];
} type_t;

typedef struct {
    void *ptr;
} complex_type_t;

void foo(complex_type_t *ct)
{
    type_t *t = ct->ptr;

    printf("%s\n", t->str);
}

int main(int argc, char const *argv[])
{
    type_t tmp;
    type_t *tmp_ptr = &tmp;

    sprintf(tmp.str, "hello-world");

    foo((complex_type_t *)&tmp_ptr);    

    return 0;
}