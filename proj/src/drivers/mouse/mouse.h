#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>

#include "../../assets/cursor.xpm"
#include "../../game/views/sprite/sprite.h"
#include "../kbc/i8042.h"
#include "../kbc/kbc.h"
#include "../utils/handlers.h"

typedef struct packet packet_t;
typedef sprite_t *mouse_ptr;

packet_t mouse_data_to_packet(uint8_t *data);
int mouse_disable_data_reporting();
int _mouse_enable_data_reporting_();
struct mouse_ev *mouse_get_event(struct packet *pp);

#endif
