#include "i8042.h"
#include "utils.h"
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

int(mouse_subscribe_int)(uint8_t *bit_no);

int(mouse_unsubscribe_int)();

void(mouse_ih)();

bool(check_in_buf)();

bool(check_out_buf)();

int(mouse_write_cmd)(uint32_t cmd, uint8_t *resp);

int(mouse_option)(uint8_t cmd);

void(makePack)(struct packet *pack);

typedef struct packet packet_t;
struct mouse_ev *mouse_get_event(struct packet *pp);
