#include "i8254.h"
#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>

static int hook_id;   // Global var to identify timer interrupts
unsigned counter = 0; // Global var for timer increments at interrupts

/*
enum timer_status_field {
  tsf_all,	// Display status byte, in hexadecimal
  tsf_initial,	// Display the initialization mode, only
  tsf_mode,	// Display the counting mode, only
  tsf_base	// Display the counting base, only
};

union timer_status_field_val {
  uint8_t byte;			// The status byte
  enum timer_init in_mode;	// The initialization mode
  uint8_t count_mode;		// The counting mode: 0, 1,.., 5
  bool bcd;			// The counting base, true if BCD
};

enum timer_init{
  INVAL_val, 		// Invalid initialization mode
  LSB_only,		// Initialization only of the LSB
  MSB_only,		// Initialization only of the MSB
  MSB_after_LSB		// Initialization of LSB and MSB, in this order
};
*/

int(timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint8_t rb_cmd = 0;

  // construir Read_Back_Command com o que queremos pedir
  //(queremos o status ent deixamos a zero por ser negado)
  rb_cmd |= TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  // agora mandamos a RB_CMD para o controlador para fazer o pedido
  sys_outb(TIMER_CTRL, rb_cmd);

  // lemos o resultado da port do timer certo
  util_sys_inb(TIMER_0 + timer, st);

  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val conf;

  // Agora temos de alterar os campos do conf para o que recebemos do timer em st
  switch (field) {
    case tsf_all:
      conf.byte = st;
      break;
    case tsf_initial:
      conf.in_mode = CONF_INIT_MODE(st);
      break;
    case tsf_mode:
      conf.count_mode = CONF_COUNT_MODE(st);

      if (conf.count_mode == 6)
        conf.count_mode = 2;
      if (conf.count_mode == 7)
        conf.count_mode = 3;

      break;
    case tsf_base:
      conf.bcd = CONF_BCD_MODE(st);
      break;
  }

  /*
  #define CONF_INIT_MODE(n) ((n & (BIT(5) | BIT(4))) >> 4)
  #define CONF_COUNT_MODE(n) ((n & (BIT(3) | BIT(2) | BIT(1))) >> 1)
  #define CONF_BCD_MODE(n) (n & BIT(0))
  */

  timer_print_config(timer, field, conf);

  return 0;
}

/*    FUNCTIONS FOR 2    */

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {

  // Freq must be >= 20 to work properly
  if (freq < 20) {
    printf("Timer freq provided isnt greater than 19");
    return 1;
  }

  // Calcular frequencia relativa em relacao ao processador
  uint16_t val = (uint16_t) (TIMER_FREQ / freq);

  // Partir a freq em parte mais e menos significativa
  uint8_t msb, lsb;
  util_get_LSB(val, &lsb);
  util_get_MSB(val, &msb);

  // Notas:
  // Make sure that you do not change the 4 least significant bits
  // This requires you to read the input timer config before you change it

  // ler conteudo atual do timer
  uint8_t st;
  timer_get_conf(timer, &st);

  // Manter os 4 LSB intactos, colocar no modo LSB + MSB e marca o timer certo
  // os bits 6,7 são os responsáveis por definir o num do timer na cmd_word
  // oa bits 4,5 servem para marcar LSB,MSB
  st = (timer << 6) | TIMER_LSB_MSB | (st & 0x0F);

  // atualizar settings do timer correto --> introduzir novo valor na port do timer que queremos para ele contar a partir daí
  sys_outb(TIMER_CTRL, st);       // escreve no control_reg o cmd_word para configurar timer
  sys_outb(TIMER_0 + timer, lsb); // escreve a primeira parte (LSB) no timer certo
  sys_outb(TIMER_0 + timer, msb); // escreve a outra metade no timer certo
  // Duplo envio por limite do barramento de 1B -->  temos de enviar lsb e dps msb

  return 0;
}

/*    FUNCTIONS FOR 3    */

int(timer_subscribe_int)(uint8_t *bit_no) {
  hook_id = *bit_no = TIMER0_IRQ; // Made up assignment. Just has to be unique (and IRQ is unique)
  sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
  return 0;
}

int(timer_unsubscribe_int)() {
  sys_irqrmpolicy(&hook_id);
  return 0;
}

void(timer_int_handler)() {
  counter++;
}
