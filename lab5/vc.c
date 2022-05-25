#include "vc.h"

/*    Functions for 1      */

int set_vc_mode(uint16_t mode) {
  return 0;
}

/*     Functions for 2     */

int(changePixelColor)(uint16_t x, uint16_t y, uint32_t color) {
  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  return 0;
}

/*    Functions for 3     */

int(video_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step) {
  return 0;
}

uint32_t(indexed_color)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t no_rectangles) {
  return 0;
}

uint32_t(direct_color)(uint32_t red, uint32_t green, uint32_t blue) {
  return 0;
}

uint32_t(R_First)(uint32_t first) {
  return 0;
}

uint32_t(G_First)(uint32_t first) {
  return 0;
}

uint32_t(B_First)(uint32_t first) {
  return 0;
}

uint32_t(R)(unsigned w, uint8_t step, uint32_t first) {
  return 0;
}

uint32_t(G)(unsigned h, uint8_t step, uint32_t first) {
  return 0;
}

uint32_t(B)(unsigned w, unsigned h, uint8_t step, uint32_t first) {
  return 0;
}

/*    Functions for 5    */

int XPMmove(xpm_map_t xpm, moveCords_t *cords, int16_t speed, uint8_t fr_rate) {
  return 0;
}

int cordsCalc(moveCords_t *cords, int16_t speed) {
  return 0;
}

/*  Auxiliar func */

int waitForEscPress() {
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

  kbd_unsubscribe_int(); // devolve controlo do kbd ao minix
  return 0;
}
