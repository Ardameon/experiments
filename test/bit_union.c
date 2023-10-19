#include <stdio.h>
#include <stdint.h>

typedef struct
{
    union
    {
        struct
        {
            uint16_t a0:1,
                     a1:1,
                     a2:1,
                     a3:1,
                     a4:1,
                     a5:1,
                     a6:1,
                     a7:1,
                     a8:1,
                     a9:1,
                     a10:1,
                     a11:1,
                     a12:1,
                     a13:1,
                     a14:1,
                     a15:1,
                     :0;
        };
        uint16_t mask;
    } u;
} bit_un_t;

int main()
{
    bit_un_t st;

    /* 0xA */
    st.u.a1 = 1;
    st.u.a3 = 1;
    /* 0xB */
    st.u.a4 = 1;
    st.u.a5 = 1;
    st.u.a7 = 1;
    /* 0xC */
    st.u.a10 = 1;
    st.u.a11 = 1;
    /* 0xC */
    st.u.a12 = 1;
    st.u.a14 = 1;
    st.u.a15 = 1;


    printf("union value: %#x\n", st.u.mask);

    return 0;
}

