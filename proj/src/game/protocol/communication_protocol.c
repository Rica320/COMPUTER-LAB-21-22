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
    else if (pro.com_status){
        byte |= (pro.message & 0x0f); // to ensure mutex
        byte |= BIT(6);
    }else {
        byte |= ((pro.message) << 1);
        if (pro.more_chars) {
            byte |= BIT(0);
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
        } else {
            pro->col = false;
        }
    }else if (byte & BIT(6)) {
        pro->com_status = true;
        pro->message = byte & 0x0f;
    }else {
        if (byte & BIT(0)) {
            pro->more_chars = true;
        } else {
            pro->more_chars = false;
        }
        pro->message = (byte >> 1) & 0x1f; 
    }

    return EXIT_SUCCESS;
}
