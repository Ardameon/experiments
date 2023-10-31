#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include "running_str.h"

#define DISPLAY_REFRESH_TO_MS 100
#define DISPLAY_INTESITY 1
#define DISPLAY_ROTATION 3
#define DISPLAY_MSG_LEN  128

int pinCS = 10;
int numberOfHorizontalDisplays = 1;
int numberOfVerticalDisplays = 1;

uint8_t *display = NULL;
char message[DISPLAY_MSG_LEN];
uint8_t loop_enabled = 1;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

void setup()
{
    matrix.setIntensity(DISPLAY_INTESITY);
    matrix.setRotation(DISPLAY_ROTATION);
    strncpy(message, "Hello world!", DISPLAY_MSG_LEN - 1);
    DisplayReset();
}

void loop()
{
    if (!display)
    {
        if (loop_enabled)
        {
            DisplayReset();
        }
        else
        {
            return;
        }
    }

    DisplayPrint(display, RS_DISPLAY_HEIGHT, RS_DISPLAY_WIDTH);

    delay(DISPLAY_REFRESH_TO_MS);

    display = (uint8_t *)RS_StringProc();

    DisplayClear();
}

void DisplayReset()
{
    RS_StringSet(message);
    display = (uint8_t *)RS_StringProc();
    DisplayClear();
}

void DisplayPrint(uint8_t *display, int height, int width)
{
    int i, j;

    for (i = 0; i < height; i++)
    {
        for (j = width - 1; j >= 0; j--)
        {
            matrix.drawPixel(i, j, ((display[i] >> j) & 1));
        }
    }

    matrix.write();
}

void DisplayClear()
{
    matrix.fillScreen(LOW);
    matrix.write();
}
