#include "i8254.h"
#include <lcom/lcf.h>
#include <lcom/timer.h>
#include <stdint.h>

/*    FUNCTIONS FOR 1    */

int(timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint8_t cmd = 0;

  // construir Read_Back_Command com o que queremos pedir
  //(queremos o status ent deixamos a zero por ser negado)
  cmd |= TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);

  // agora mandamos a RB_CMD para o controlador para fazer o pedido
  sys_outb(TIMER_CTRL, cmd);

  // lemos o resultado da port do timer certo
  util_sys_inb(TIMER_0 + timer, st);

  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val val;

  // Agora temos de alterar os campos do conf para o que recebemos do timer em st
  switch (field) {
    case tsf_all:
      val.byte = st;
      break;

    case tsf_initial:
      val.in_mode = (st & (BIT(5) | BIT(4))) >> 4;
      break;

    case tsf_mode:
      val.count_mode = (st & (BIT(3) | BIT(2) | BIT(1))) >> 1;
      if (val.count_mode > 5)
        val.count_mode &= ~BIT(2);
      break;

    case tsf_base:
      val.bcd = st & BIT(0);
      break;

    default:
      break;
  }

  timer_print_config(timer, field, val);

  return 0;
}

/*    FUNCTIONS FOR 2    */

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {

  // Freq must be > 20 to work properly
  if (freq < 20)
    return 1;

  // Calcular frequencia relativa em relacao ao processador
  uint16_t rel_freq = (uint16_t) (TIMER_FREQ / freq);

  // Partir a freq em parte mais e menos significativa
  uint8_t lsb, msb;
  util_get_LSB(rel_freq, &lsb);
  util_get_MSB(rel_freq, &msb);

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

  // atualizar settings do timer correto --> introduzir novo valor na port do timer que queremos
  sys_outb(TIMER_CTRL, st); // escreve no control_reg o cmd_word para configurar timer

  sys_outb(TIMER_0 + timer, lsb); // escreve a primeira parte (LSB) no timer certo
  sys_outb(TIMER_0 + timer, msb); // escreve a outra metade no timer certo
  // Duplo envio por limite do barramento de 1B -->  temos de enviar lsb e dps msb

  return 0;
}

/*    FUNCTIONS FOR 3    */

unsigned counter = 0;
static int hook_id;

int(timer_subscribe_int)(uint8_t *bit_no) {
  // Made up assignment. Just has to be unique (and IRQ is unique)
  hook_id = *bit_no = TIMER0_IRQ;
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
