#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct st1 {
    char a;
    char *b;
};

struct st2 {
    char a;
    char *b;
} __attribute__((packed));

#define COUNT 400000000
// #define CLOCK CLOCK_MONOTONIC
#define CLOCK CLOCK_PROCESS_CPUTIME_ID

void alloc1(void)
{
    struct timespec ts;
    long diff;
    struct st1 *arr;

    clock_gettime(CLOCK, &ts);

    diff = ts.tv_nsec;

    arr = malloc(COUNT * sizeof(*arr));

    // printf("time: %ld nsec\n", ts.tv_nsec);

    clock_gettime(CLOCK, &ts);
    // printf("time: %ld nsec\n", ts.tv_nsec);

    diff = ts.tv_nsec - diff;

    printf("%s interval: %ld nsec\n", __func__, diff);
}

void alloc2(void)
{
    struct timespec ts;
    long diff;
    struct st2 *arr;

    clock_gettime(CLOCK, &ts);

    diff = ts.tv_nsec;

    arr = malloc(COUNT * sizeof(*arr));

    // printf("time: %ld nsec\n", ts.tv_nsec);

    clock_gettime(CLOCK, &ts);
    // printf("time: %ld nsec\n", ts.tv_nsec);

    diff = ts.tv_nsec - diff;

    printf("%s interval: %ld nsec\n", __func__, diff);
}

int main(int argc, char const *argv[])
{
    alloc1();
    alloc2();
    return 0;
}