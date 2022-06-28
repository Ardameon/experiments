/* compile with -Wswitch-enum */

#include <stdio.h>

typedef enum {
    E_CONST_MIN = 0,
    E_CONST_1 = E_CONST_MIN,
    E_CONST_2,
    E_CONST_3,
    E_CONST_4,
    E_CONST_MAX = E_CONST_4
} type_e;

int main(int argc, char const *argv[])
{
    type_e en = E_CONST_4;

    for (en = E_CONST_MIN; en <= E_CONST_MAX; ++en)
    {
        switch (en)
        {
            case E_CONST_1:
            case E_CONST_2:
            // case E_CONST_3:
            case E_CONST_4:
                printf("en: %d\n", en);
                break;

            default:
                printf("default en: %d\n");

        }
    }

    return 0;
}