#include <lcom/lcf.h>

#ifndef _LCOM_COM_PROTOCOL_H_
#define _LCOM_COM_PROTOCOL_H_

typedef struct Protocol {
    uint8_t origin;
    uint8_t message;
    uint8_t dest;
    bool move;
    bool more_chars;
    bool col;
    bool com_status;
}Protocol;

enum Communication_Status {
    no_one,
    waiting,
    connected,
};

uint8_t encode_protocol(Protocol pro);
int decode_protocol(Protocol * pro, uint8_t byte);

#endif
