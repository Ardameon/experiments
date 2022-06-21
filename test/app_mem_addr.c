#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int glb;

void func()
{
	int a;
	void **pt = (void **)&a;

	pt++;

	for (a = 0; a < 10; a++)
	{

		printf("func:%p pt:%p *pt:%p\n", func, pt, *pt);
		pt++;
	}
}

int main(int argc, char const *argv[])
{
    static int st;

    // int a;
    int *p = malloc(sizeof(int));
    int t;
    void **vp = (void **)&p;

    vp++;

    printf("stack1:  %p\n"
           "stack2:  %p\n"
           "code:    %p\n"
           "heap:    %p\n"
           "static:  %p\n" 
           "global:  %p\n"
           "main:    %p (val: %p)\n", 
           &p, &t, main, p, &st, &glb, vp, *vp);

	func();

    // memset(&t, 0, 14);

    return 0;
}
