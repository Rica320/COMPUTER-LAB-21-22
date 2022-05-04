#include "i8042.h"
#include "i8254.h"
#include "kbd.h"
#include <lcom/lab3.h>
#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

extern uint8_t scancode; // keyboard's scancode
extern unsigned count_inb;

int(kbd_test_scan)() {

  int ipc_status;
  message msg;
  uint8_t bit_no;

  // Substituir minix pelo nosso interrupt handler
  kbd_subscribe_int(&bit_no);

  // vars para ler os bytes scancode
  bool another_read = false;
  uint8_t codes[2];

  while (scancode != ESC_BREAKCODE) {
    // wait for any kind of message
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("Driver_receive failed\n");
      continue;
    }

    // if there is a message from an i/o
    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE)
      if (msg.m_notify.interrupts & BIT(bit_no)) {

        kbc_ih(); // chamar o nosso handler

        if (!another_read) {
          codes[0] = scancode; // le byte

          // deteta se o byte lido é MSB de um scancode de 2B
          if (scancode == MSB_2B_SCANCODE)
            another_read = true; // Marca para na proxima iteracao ler outro byte (lsb)
          else
            kbd_print_scancode(!(scancode & BIT(7)), 1, codes); // funcão do stor
        }
        else {
          // ha um segundo byte a ler
          codes[1] = scancode;  // guarda segundo byte
          another_read = false; // desativa flag de segunda leitura
          kbd_print_scancode(!(BIT(7) & scancode), 2, codes);
        }
      }
  }

  kbd_unsubscribe_int();          // devolve controlo do kbd ao minix
  kbd_print_no_sysinb(count_inb); // só para mostrar nº chamadas
  return 0;
}

int(kbd_test_poll)() {

  // use polling instead of interrupts
  uint8_t codes[2], size;

  // faz polling até aparecer breakcode ESC
  while (codes[0] != ESC_BREAKCODE) {

    kbd_poll(codes, &size);

    // print the code read
    kbd_print_scancode(!(codes[size - 1] & BIT(7)), size, codes);
  }

  kbd_restore();                  // devolve controlo ao minix
  kbd_print_no_sysinb(count_inb); // print de chamadas inb
  return 0;
}

extern unsigned counter; // Timer counter

int(kbd_test_timed_scan)(uint8_t n) {

  // Basicamente copiar o codigo da primeira funcao
  // e adicionar tambem o subscribe e handler do timer

  int ipc_status;
  message msg;
  uint8_t bit_no_kbd, bit_no_timer;

  // Substituir minix pelo nosso interrupt handler
  kbd_subscribe_int(&bit_no_kbd);

  // Subscrever timer interrutps
  timer_subscribe_int(&bit_no_timer);

  // vars para ler os bytes scancode
  bool another_read = false;
  uint8_t bytes[2];

  while ((scancode != ESC_BREAKCODE)) {

    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("driver_receive failed\n");
      continue;
    }

    if (is_ipc_notify(ipc_status))
      if (_ENDPOINT_P(msg.m_source) == HARDWARE) {

        // handle kbd interrupt
        if (msg.m_notify.interrupts & BIT(KBD_IRQ)) {

          // reset the afk timer counter
          counter = 0;

          kbc_ih(); // chamar o nosso handler

          if (!another_read) {

            bytes[0] = scancode; // le byte

            if (scancode == MSB_2B_SCANCODE) // deteta se o byte lido é MSB de um scancode de 2B
              another_read = true;           // Marca para na proxima iteracao ler outro byte (lsb)
            else
              kbd_print_scancode(!(BIT(7) & scancode), 1, bytes); // funcão do stor
          }
          else {
            // ha um segundo byte a ler
            bytes[1] = scancode;  // guarda segundo byte
            another_read = false; // desativa flag de segunda leitura
            kbd_print_scancode(!(BIT(7) & scancode), 2, bytes);
          }
        }

        // handle timer interrupt
        if (msg.m_notify.interrupts & BIT(TIMER0_IRQ)) {

          timer_int_handler(); // chamar o nosso handler

          if (counter == n * 60)
            break;
        }
      }
  }

  kbd_unsubscribe_int();          // devolve controlo do kbd ao minix
  kbd_print_no_sysinb(count_inb); // só para mostrar nº chamadas
  return 0;
}
