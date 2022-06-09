#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <lcom/lcf.h>

#include "../utils/handlers.h"
#include "i8042.h"

uint8_t scancode[2];
uint8_t mouse_data[3];
uint8_t mouse_received_count;
bool mouse_packet_completed;
int scancode_sz;
bool two_byte_scancode;
bool error_flag;

int(subscribe_kbc_interrupt)(uint8_t bit_no, int *hook_id, int kbc); // CHANGE THIS FROM HERE
int(unsubscribe_interrupt)(int *hook_id);
int(kbc_read_i)();
bool(kbc_get_error)();
void(kbc_ih)(void);
bool(kbc_ready)();
bool(kbc_mouse_ready)();
void(kbc_get_scancode)(unsigned char *scan, int *scan_sz);
void(kbc_get_mouse_data)(unsigned char *scan);
int(kbc_read)(uint8_t *code);

int(kbc_issue_cmd)(uint8_t cmd);
int(kbc_read_cmd)(uint8_t *cmd);
int(kbc_write_cmd)(uint8_t cmd);
int(kbc_send_mouse_cmd)(uint8_t cmd);
int(kbc_cmd_arg)(uint8_t arg);
int(kbc_check_cmd)();

#endif
