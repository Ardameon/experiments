#include <stdio.h>

typedef enum
{
    E_C0,
    E_C1,
    E_C2,
    E_C3,
    E_C4,
    E_C5,
    E_CMAX
} stat_str_e;


const char *str_arr[E_CMAX] =
{
    [E_C4] = "Enum string 4",
    [E_C0] = "Enum string 0",
    [E_C1] = "Enum string 1",
    [E_C3] = "Enum string 3",
    [E_C2] = "Enum string 2",
};


int main(void)
{
    int i = 0;

    for (; i < E_CMAX; i++)
    {
        printf("str[%d]: %s\n", i, str_arr[i]);
    }

    return 0;
}
