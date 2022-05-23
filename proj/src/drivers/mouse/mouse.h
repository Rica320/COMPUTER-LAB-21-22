#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>

#include "../../game/sprite/sprite.h"
#include "../../assets/cursor.xpm"

typedef struct packet packet_t;

packet_t mouse_data_to_packet(uint8_t* data);
int mouse_disable_data_reporting();
int _mouse_enable_data_reporting_();

typedef sprite_t * mouse_ptr;

struct mouse_ev* mouse_get_event(struct packet *pp);

#endif
