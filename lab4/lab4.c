#include "i8042.h"
#include "mouse.h"
#include <lcom/lab4.h>
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

int(kbd_test_scan)() {

  int ipc_status;
  message msg;
  uint8_t bit_no;

  // Substituir minix pelo nosso interrupt handler
  mouse_subscribe_int(&bit_no);

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

  mouse_unsubscribe_int(); // devolve controlo do kbd ao minix
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {

  // Basicamente copiar o codigo da primeira funcao
  // e adicionar tambem o subscribe e handler do timer

  int ipc_status;
  message msg;
  uint8_t bit_no_mouse;

  // Substituir minix pelo nosso interrupt handler
  mouse_subscribe_int(&bit_no_mouse);

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
        if (msg.m_notify.interrupts & BIT(bit_no_mouse)) {

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
      }
  }

  mouse_unsubscribe_int(); // devolve controlo do kbd ao minix

  return 0;
}
