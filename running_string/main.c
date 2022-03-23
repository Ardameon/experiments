#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "running_str.h"

void DisplayPrint(uint8_t *display, int height, int width)
{
    int i, j;

    for (i = 0; i < height; i++)
    {
        for (j = width - 1; j >= 0; j--)
        {
            printf("%c ", ((display[i] >> j) & 1) ? 'X' : '.');
        }

        printf("\n");
    }
}

int main(void)
{
    uint8_t *display;

    RS_StringSet("Hello world");

    display = RS_StringProc();

    while (display)
    {
        system("clear");
        DisplayPrint(display, RS_DISPLAY_HEIGHT, RS_DISPLAY_WIDTH);

        printf("\n");

        fflush(stdout);

        display = RS_StringProc();
        sleep(1);
    }

    return 0;
}