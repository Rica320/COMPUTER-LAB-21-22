#include <lcom/lcf.h>

enum mouse_mode {stream, remote};
void mouse_config();
void mouse_reconfig();

int mouse_subscribe_int(int *bit_no);
int mouse_unsubscribe_int();

uint8_t mouse_cmd_read();
int mouse_send_cmd(uint8_t byte);

//Mouse Commands
int mouse_reset();
int mouse_set_default();
int mouse_set_mode(enum mouse_mode mode);
int mouse_enable_data_rep();
int mouse_disable_data_rep();

uint8_t mouse_read_packet_byte();
void mouse_parse_packet(uint8_t packet[3]);

int (util_sys_inb)(int port, uint8_t *value);
