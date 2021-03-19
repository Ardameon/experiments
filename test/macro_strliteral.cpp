#include <stdio.h>

#define MACRO_STR "bar"
#define STR_MACRO "/foo/" MACRO_STR

int main(int argc, char const *argv[])
{
    printf("'%s'\n", STR_MACRO);
    return 0;
}