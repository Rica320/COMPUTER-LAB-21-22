#include <lcom/lab5.h>
#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "vc.h"

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {

  set_vc_mode(mode);

  sleep(delay);

  vg_exit();

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {

  set_vc_mode(mode);

  vg_draw_rectangle(x, y, width, height, color);

  waitForEscPress();
  vg_exit();
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

  set_vc_mode(mode);

  video_pattern(no_rectangles, first, step);

  waitForEscPress();
  vg_exit();
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {

  set_vc_mode(INDEXED_COLOR);

  xpm_image_t img;
  uint8_t *map = xpm_load(xpm, XPM_INDEXED, &img);

  for (unsigned i = 0; i < img.height; i++)
    for (unsigned j = 0; j < img.width; j++)
      changePixelColor(x + j, y + i, *map++);

  waitForEscPress();
  vg_exit();
  return 0;
}

extern unsigned counter;

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {

  set_vc_mode(INDEXED_COLOR);

  xpm_image_t img;
  uint8_t *map = xpm_load(xpm, XPM_INDEXED, &img);

  for (unsigned i = 0; i < img.height; i++)
    for (unsigned j = 0; j < img.width; j++)
      changePixelColor(xi + j, yi + i, *map++);

  moveCords_t cords = {.xi = xi, .yi = yi, .xf = xf, .yf = yf, .newX = xi, .newY = yi};

  // ============================================================================

  int ipc_status, r;
  message msg;

  // vars para ler os bytes scancode
  bool another_read = false;
  uint8_t codes[2];

  uint8_t bit_no_timer, bit_no_kbd;

  // subscrever interrupts
  timer_subscribe_int(&bit_no_timer);
  kbd_subscribe_int(&bit_no_kbd);

  while (scancode != ESC_BREAKCODE) {

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }

    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE) {

      // KBD Interrupt
      if (msg.m_notify.interrupts & BIT(bit_no_kbd)) {

        kbc_ih(); // chamar o nosso handler

        if (!another_read) {
          codes[0] = scancode;
          if (scancode == MSB_2B_SCANCODE)
            another_read = true;
        }
        else {
          codes[1] = scancode;
          another_read = false;
        }
      }

      // Timer Interrupt
      if (msg.m_notify.interrupts & BIT(bit_no_timer))
        if (cords.newX != xf || cords.newY != yf) {

          timer_int_handler();

          if (counter % (sys_hz() / fr_rate) == 0)
            XPMmove(xpm, &cords, speed, fr_rate);
        }
    }
  }

  timer_unsubscribe_int();
  kbd_unsubscribe_int();

  // ============================================================================

  waitForEscPress();
  vg_exit();
  return 0;
}
