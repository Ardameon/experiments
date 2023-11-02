#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include "running_str.h"

#define DISPLAY_REFRESH_TO_MS 100
#define DISPLAY_INTESITY 1
#define DISPLAY_ROTATION 3
#define DISPLAY_MSG_LEN  256
#define RX_BUF_LEN       DISPLAY_MSG_LEN


int pinCS = 10;
int numberOfHorizontalDisplays = 1;
int numberOfVerticalDisplays = 1;

uint8_t *display = NULL;
char message[DISPLAY_MSG_LEN];
uint8_t loop_enabled = 1;
uint8_t serial_rx_buf[RX_BUF_LEN];
uint8_t rx_ready_cnt, rx_recv_cnt;

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

void setup()
{
    matrix.setIntensity(DISPLAY_INTESITY);
    matrix.setRotation(DISPLAY_ROTATION);
    strncpy(message, "Hello world!", DISPLAY_MSG_LEN - 1);
    DisplayReset();
    Serial.begin(9600);
}

void loop()
{
    if (Serial.available())
        UpdateMessageFromSerial();

    if (!display)
    {
        if (loop_enabled)
        {
            Serial.println("Reset Display");
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

void UpdateMessageFromSerial()
{
    while ((rx_ready_cnt = Serial.available()))
    {
        rx_recv_cnt += Serial.readBytes(&serial_rx_buf[rx_recv_cnt], RX_BUF_LEN - rx_recv_cnt - 1);
    }

    Serial.println((char *)serial_rx_buf);
    RS_StringStop();

    strncpy(message, serial_rx_buf, DISPLAY_MSG_LEN - 1);
    DisplayReset();

    memset(serial_rx_buf, 0, RX_BUF_LEN);
    rx_recv_cnt = 0;
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
