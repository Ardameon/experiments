#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
 * To build it with sanitizer use this:
 *
 * gcc sanitizer.c -fsanitize=address -static-libasan -g -fno-omit-frame-pointer
 */

void func()
{
    char *p;
    int counter = 0;

    while (counter < 5)
    {
        p = malloc(25);
        sprintf(p, "String step %d", counter++);
//        p[44] = 'f';
        printf("%s\n", p);
//        free(p);
        sleep(1);
    }
}

int main()
{
    func();
    func();

    return 0;
}
