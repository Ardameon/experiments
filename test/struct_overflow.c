#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t device;
    uint8_t slot;
    uint16_t port;
} CSPG_PORT;

CSPG_PORT port;

void print_port()
{
    printf("dev:%u slot:%u port:%u\n", port.device, port.slot, port.port);
}

int main(int argc, char const *argv[])
{
    
    uint8_t a = 250;
    uint16_t b = 1500;
    int c = 78000;

    print_port();

    port.port = a;
    print_port();

    port.port = b;
    print_port();

    port.port = c;
    print_port();

    return 0;
}