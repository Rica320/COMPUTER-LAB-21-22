#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lcf.h>

typedef struct packet packet_t;

packet_t mouse_data_to_packet(uint8_t* data);
int mouse_disable_data_reporting();
int _mouse_enable_data_reporting_();

#endif
