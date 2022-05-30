#include "communication_protocol.h"

uint8_t encode_protocol(Protocol pro) {

    uint8_t byte = 0;

    if (pro.move)
    {
        byte |= (pro.origin & 0x07);
        byte |= ((pro.dest & 0x07) << 3);
        byte |= BIT(7);
        if (pro.col) {
            byte |= BIT(6);
        }
    }

    return byte;
}

int decode_protocol(Protocol * pro, uint8_t byte) {

    if (byte & BIT(7))
    {
        pro->origin = byte & ((1 << 3) - 1);
        pro->dest = (byte >> 3) & ((1 << 3) - 1);
        pro->move = true;
        if (byte & BIT(6))
        {
            pro->col = true;
        }
        
    }

    return EXIT_SUCCESS;
}
