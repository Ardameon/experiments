#include <stdio.h>
#include <stdint.h>

typedef struct
{
    union
    {
        struct
        {
            uint8_t a:1,
                    b:1,
                    c:1,
                    d:1,
                    :0;
        };
        uint8_t mask;
    } u;
} bit_un_t;

int main()
{
    bit_un_t st;

    st.u.a = 1;
    st.u.b = 0;
    st.u.c = 0;
    st.u.d = 1;

    printf("union value: %#x\n", st.u.mask);

    return 0;
}

