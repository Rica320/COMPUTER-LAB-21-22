#include "timer.h"


// used for interrupts
static int hook_id;
unsigned int timer_counter = 0;


int(timer_set_frequency)(uint8_t timer, uint32_t freq) {

	if(freq<=19){
		printf("Freq cant be <=19\n");
		return 1;
	}

	uint16_t val = (uint16_t)(TIMER_FREQ / freq);
	uint8_t msb, lsb;
	utils_get_LSB(val, &lsb);
	utils_get_MSB(val, &msb);

	// ler configuracao do timer antes de alterar
	uint8_t config;
	timer_get_conf(timer, &config); // talvez catch de return 1;

	// Guardar os 4 LSB e colocar no modo LSB followed by MSB
	config = TIMER_LSB_MSB | ((TIMER_BCD | BIT(3) | TIMER_SQR_WAVE) & config);

	// marcar o timer correto
	config |= (timer << 6);

	// atualizar settings do timer correto --> introduzir novo valor na port do timer que queremos para ele contar a partir daí
	sys_outb(TIMER_CTRL, config);
	sys_outb(TIMER_0 + timer, lsb); // talvez catch de return 1;
	sys_outb(TIMER_0 + timer, msb);
	// Duplo envio por limite do barramento de 1B -->  temos de enviar lsb e dps msb

	return 0;
}


int (timer_subscribe_int)(uint8_t *bit_no) {
  hook_id = *bit_no =  0;

  return (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != 0);
}

int (timer_unsubscribe_int)() {
  return (sys_irqrmpolicy(&hook_id) != 0);
}

void (timer_int_handler)() {
  timer_counter++;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {

  uint8_t rb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | (TIMER_RB_SEL(timer));
  sys_outb(TIMER_CTRL, rb_cmd);
  utils_sys_inb(TIMER_0 + timer /*Port adress*/, st);

  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  union timer_status_field_val tsf;

  switch (field) {
    case tsf_all:
      tsf.byte = st; 
      break;
    case tsf_base:
      tsf.bcd = (st & TIMER_BCD) == 0;
      break;
    case tsf_mode:
      tsf.count_mode = (TIMER_SQR_WAVE | BIT(3)) & st;
      break;
    case tsf_initial:
      tsf.in_mode = st;
      break;
    default:
      break;
  }

  return timer_print_config(timer, field, tsf);
}



int(timer_print_config)(uint8_t timer, enum timer_status_field field) {

  uint8_t st;
  timer_get_conf(timer, &st) ;
  timer_display_conf(timer, st, field);

  return 0;
}


