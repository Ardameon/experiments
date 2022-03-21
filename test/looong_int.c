#include <stdint.h>

int main(int argc, char const *argv[])
{
    uint64_t msg_id = 3;
    int telcoId = 2;

    // uint64_t mask = 0xffff00ff;
    // mask = (mask << 32) | 0xffffffff;

    uint64_t mask = 0xff;

    msg_id = (msg_id & ~(mask << 40)) | (((uint64_t)(telcoId & 0xff)) << 40);
    
    return 0;
}