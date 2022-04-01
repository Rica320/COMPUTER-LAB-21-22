#include "i8042.h"
#include "utils.h"
#include <lcom/lab3.h>
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern uint8_t scancode, stat;
extern int count_inb;

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;

  lcf_cleanup();
  return 0;
}

int(kbd_test_scan)() {

  /* PRINT MAKE AND BREAK CODES READ FROM KBC

    - Desativar os interrupts internos do minix
    - Criar os nosso interrupt (SUBSCREVER kbc interrupts)

    sys_irqsetpolicy(1,IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id);

    - Esperar por interrupts usando driver_receive() loop
                (similar to that of lab 2)

    - Tentar esperar por interrupts:
          tickdelay(micros_to_ticks(DELAY_US));

    - Perante interrupt, read scancode from KBD_OUT_BUF
      ---> read one byte per interrupt (communication too slow)

    - Verficiar se houve erro (ler status register)
        ---> bits 6,7 sao de erro
        --- if bit 1 ativo, não escrever nas ports

    - int kbd_print_scancode(bool make, uint8_t size, uint8_t bytes[])

            make
            indicates whether this is a make or a break code

            size
            is the size of the scancode in bytes

            bytes
            is an array with the bytes of the scancode, in order, i.e. the first byte will be in the first element of the array, the second byte, if any, in the array's second element and so on


    -  Terminate when reads ESC_BREAKCODE

    -  Reativar as interrupcoes do minix para poder usar o teclado:
        sys_irqrmpolicy(hook_id)

    - Fazer print da contagem de sys_inb :      int kbd_print_no_sysinb(uint32_t cnt);

    - Dar return 0;


  */

  // vars para fazer interrupções funcionar
  int ipc_status, r;
  uint32_t irq_set = BIT(1);
  uint8_t bit_no;
  message msg;

  // vars para ler os bytes scancode
  bool another_read = false;
  uint8_t bytes[2];

  // subscreve as interrupcoes
  if (kbd_subscribe_int(&bit_no) != 0)
    return 1;

  // ciclo continua até ESC ser pressionado
  while (scancode != ESC_BREAKCODE) {

    // tenta receber alguma coisa
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("Driver fail: %d", r);
      continue;
    }

    // verifica se a informação recebida é uma interrupção do tipo que estamos a espera (keyboard)
    if ((is_ipc_notify(ipc_status)) && (_ENDPOINT_P(msg.m_source) == HARDWARE))
      if (msg.m_notify.interrupts & irq_set) {

        kbc_ih(); // chamar o nosso interrupt handler

        if (!another_read) {

          bytes[0] = scancode;             // le byte
          if (scancode == MSB_2B_SCANCODE) // deteta se o byte lido é MSB de um scancode de 2B
            another_read = true;           // Ativa flag para na proxima iteracao do loop ler outro byte (lsb)
          else
            kbd_print_scancode(!(SR_PARITY_ERROR & scancode), 1, bytes);
        }
        else {
          // ha um segundo byte a ler
          bytes[1] = scancode; // guarda segundo byte
          kbd_print_scancode(!(SR_PARITY_ERROR & scancode), 2, bytes);
          another_read = false; // desativa flag de segunda leitura
        }
      }
  }

  kbd_unsubscribe_int();          // devolver controlo das interrupcoes ao minix para kbd funcionar
  kbd_print_no_sysinb(count_inb); // fazer print do numero de chamadas de sys_inb (pedido pelo stor)
  return 0;
}

int(kbd_test_poll)() {

  // use polling instead of interrupts
  // we'll continusly check the kbd ports in an interval

  uint8_t code[2], size = 0;

  while (code[0] != ESC_BREAKCODE) {
    kbd_poll(code, &size);
    kbd_print_scancode(!(code[size - 1] & SR_PARITY_ERROR), size, code);
  }

  kbd_restore();

  kbd_print_no_sysinb(count_inb);

  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
