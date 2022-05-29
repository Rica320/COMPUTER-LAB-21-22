#include <lcom/lcf.h>

typedef struct Protocol {
    uint8_t origin;
    uint8_t dest;
    bool move;
}Protocol;

enum Communication_Status {
    no_one,
    waiting,
    connected,
};

uint8_t encode_protocol(Protocol pro);
int decode_protocol(Protocol * pro, uint8_t byte);
