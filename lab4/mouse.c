#include <lcom/lcf.h>

#include "mouse.h"

static int hook_id;
bool packet_error = false, full_packet = false;
int packet_byte_counter = 0;
uint8_t packet[3];

void mouse_config() {
  mouse_set_mode(stream); //Caso não esteja em Stream Mode, mudar o modo
  mouse_enable_data_reporting();
  mouse_set_mode(remote);
}

void mouse_reconfig(){
  mouse_set_mode(stream);
  mouse_disable_data_rep();
}

int mouse_subscribe_int(int *bit_no){
  hook_id = *bit_no;
  sys_irqsetpolicy(12,IRQ_REENABLE|IRQ_EXCLUSIVE,&hook_id);
  return 0;
}

int mouse_unsubscribe_int(){
  sys_irqrmpolicy(&hook_id);
  return 0;
}

void (mouse_ih)(){
  uint8_t packet_byte = mouse_read_packet_byte();
  packet_byte_counter++;
  if (packet_error || (packet_byte_counter == 1 && (packet_byte|BIT(3))>>3)!=1){  //se der erro
    packet_byte_counter = 0;
    packet_error = false;
    mouse_read_packet_byte();  //começar a ler o packet novamente
  }
  else{
    packet[packet_byte_counter-1] = packet_byte;
  }
  if (packet_byte_counter == 3){
    packet_byte_counter = 0;
    full_packet = true;
  }
}

uint8_t mouse_cmd_read(){//usado para ler o command byte do mouse
  sys_outb(0x64,0x20); //Escreve comando 0x20 na porta 0x64
  uint8_t *cmd_byte = NULL;
  util_sys_inb(0x60,cmd_byte); //Lê command byte da porta 0x60 para uint8_t
  return *cmd_byte;
}

int mouse_send_cmd(uint8_t byte){ //usado para enviar comandos do KBC para o mouse
  sys_outb(0x64,0xD4); //escreve 0xD4 na porta 0x64
  sys_outb(0x60,byte); //escreve o byte que identifica o  comando na porta 0x60
  return 0;
}

int mouse_reset(){
  mouse_send_cmd(0xFF);
  return 0;
}

int mouse_set_default(){
  mouse_send_cmd(0xF6);
  return 0;
}

int mouse_enable_data_rep(){
  mouse_send_cmd(0xF4); //ativa o data reporting
  return 0;
}

int mouse_disable_data_rep(){
  mouse_send_cmd(0xF5); //desativa o data reporting
  return 0;
}

int mouse_set_mode(enum mouse_mode mode){
  switch(mode){
    case stream:
      mouse_send_cmd(0xEA);
      break;
    case remote:
      mouse_send_cmd(0xF0);
      break;
  }
  return 0;
}

uint8_t mouse_read_packet_byte(){
  mouse_send_cmd(0xEB); //solicita packet ao mouse
  uint8_t *ack_byte = NULL, *packet_byte = NULL;
  util_sys_inb(0x60, ack_byte);
  if (*ack_byte == 0xFA){
    tickdelay(25);
    util_sys_inb(0x60, packet_byte);
    return *packet_byte;
  }
  else{
    packet_error = true;
    return 1;
  }
}

void mouse_parse_packet(uint8_t packet[3]){
  printf("Parsing packet...\n");
  struct packet *pp = NULL;
  pp->bytes[0] = packet[0];
  pp->bytes[1] = packet[1];
  pp->bytes[2] = packet[2];
  pp->delta_x = packet[1];
  pp->delta_y = packet[2];
  pp->lb = packet[0] | BIT(0);
  pp->rb = (packet[0] | BIT(1))>>1;
  pp->mb = (packet[0] | BIT(2))>>2;
  pp->x_ov = (packet[0] | BIT(6))>>6; 
  pp->y_ov = (packet[0] | BIT(7))>>7;
  mouse_print_packet(pp);
}
