#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

//used for interrupts
static int hook_id;
unsigned int counter = 0;

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {

  // freq must be > 19		
  /*
  Changes the operating frequency of a timer.
  Must use the read-back command so that it does not change the 4 LSBs (mode and BCD/binary) of the timer's control word.
  */

  // Calcular frequencia relativa em relacao ao processador
  //  e calcular o msb e lsb dessa freq rel
  uint16_t val = (uint16_t)(TIMER_FREQ / freq);
  uint8_t msb, lsb;
  util_get_LSB(val, &lsb);
  util_get_MSB(val, &msb);

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
  counter++;
}





int(timer_get_conf)(uint8_t timer, uint8_t *st) {

  // construir read back cmd
  /*Define que é um Read Back*/
  /* Ativar bit do Count pq so queremos as config --> so queremos status ativado a 0*/
  /*Seleciona o timer que a q queremos aceder*/
  uint8_t rb_cmd = TIMER_RB_CMD | TIMER_RB_COUNT_ | (TIMER_RB_SEL(timer));

  // escrever no registo de controlo rb_cmd para dat indicação ao timer
  // para deixar pronto no seu registo port o que queremos (escolhido atraves da rb_cmd)
  sys_outb(TIMER_CTRL, rb_cmd);

  // ler da port o valor que queremos
  util_sys_inb(TIMER_0 + timer /*Port adress*/, st);

  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {

  // temos em ST o conteudo retirado do TIMER
  //  ver table 3 -- > timer's status byte format

  /*
          7	Output
          6	Null Count
          5, 4	Type of Access
          3,2,1	Programmed Mode
          0	BCD
  */

  // criar uma união onde guardar os valores vindos de ST
  union timer_status_field_val tsf;

  /*
      union timer_status_field_val {
          uint8_t byte;		           The status byte
          enum timer_init in_mode;	 The initialization mode
          uint8_t count_mode;		     The counting mode: 0, 1,.., 5
          bool bcd;			             The counting base, true if BCD
      }


    enum timer_status_field{
        tsf_all 	    configuration/status
        tsf_initial 	timer initialization mode
        tsf_mode 	    timer counting mode
        tsf_base 	    timer counting base
    }
  */

  switch (field) {

    case tsf_all:
      tsf.byte = st; // guardar tudo e ocupar a totalidade da union com o conteudo integral de st
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


