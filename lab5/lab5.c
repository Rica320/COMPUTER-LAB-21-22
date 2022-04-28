// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include <lcom/video_gr.h>

#include "handlers.h"
#include "video_graphic.h"
#include "keyboard.h"
#include "i8042.h"

#include "rgb.h"
#include "lcom/pixmap.h"

#include "benfica.h"

extern xpm_row_t const benfica[];

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  
  video_graphic_init(mode);
  sleep(delay);
  CHECKCall(vg_exit());

  return EXIT_SUCCESS;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  
  video_graphic_init(mode);
  CHECKCall(vg_draw_rectangle(x,y,width,height,color));

  uint8_t kbc_bit_no = 1;
  int kbc_hook_id = 0, ipc_status;
  bool esc_pressed = false, r;
  uint16_t irq_set = BIT(kbc_bit_no);

  message msg;
  
  unsigned char scan[2];
  int scan_size;

  CHECKCall(subscribe_kbc_interrupt(kbc_bit_no, &kbc_hook_id));

  while (!esc_pressed) { 

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) { 
            kbc_ih();
            if (!kbc_get_error()) {
              if (kbc_ready()) {
                kbc_get_scancode(scan, &scan_size);
                if (scan[scan_size - 1] == ESC_BREAK_CODE) {
                  esc_pressed = true;
                }
              }
            }
          }
          break;
        default:
          break;
      }
    }
    else {
    }
  }

  CHECKCall(unsubscribe_interrupt(&kbc_hook_id));
  CHECKCall(vg_exit());

  return EXIT_SUCCESS;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  video_graphic_init(mode);


  uint16_t width = (get_hres() / no_rectangles);
  uint16_t height = (get_vres() / no_rectangles);
  //uint32_t color = first;

  /*
    What if it passes the number of hres and vres , TODO
  */

  RGB color = RGB_new(first), first_r = RGB_new(first);

printf("%x \n", color.getRed(&color));
        printf("%x \n", color.getGreen(&color));
        printf("%x \n", color.getBlue(&color));

  for (uint16_t i = 0; i < no_rectangles; i++)
  {
    for (uint16_t j = 0; j < no_rectangles; j++)
    {
      if (is_indexed_mode(mode))
      {
        color.value = (first + (i * no_rectangles + j) * step) % (1 << get_bits_per_pixel()) ; 
      }else {

        color.setRed(&color, (first_r.getRed(&first_r) + j * step) % (1 << getRedMaskSize()));
        color.setGreen(&color ,(first_r.getGreen(&first_r) + i * step) % (1 << getGreenMaskSize()));
        color.setBlue(&color , (first_r.getBlue(&first_r) + (j + i) * step) % (1 << getBlueMaskSize()));
      }
      
      CHECKCall(vg_draw_rectangle(j * width, i * height, width, height, color.value));
    }
  }


  uint8_t kbc_bit_no = 1;
  int kbc_hook_id = 0, ipc_status;
  bool esc_pressed = false, r;
  uint16_t irq_set = BIT(kbc_bit_no);

  message msg;
  
  unsigned char scan[2];
  int scan_size;

  CHECKCall(subscribe_kbc_interrupt(kbc_bit_no, &kbc_hook_id));

  while (!esc_pressed) { 

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) { 
            kbc_ih();
            if (!kbc_get_error()) {
              if (kbc_ready()) {
                kbc_get_scancode(scan, &scan_size);
                if (scan[scan_size - 1] == ESC_BREAK_CODE) {
                  esc_pressed = true;
                }
              }
            }
          }
          break;
        default:
          break;
      }
    }
    else {
    }
  }

  CHECKCall(unsubscribe_interrupt(&kbc_hook_id));
  CHECKCall(vg_exit());

  return EXIT_SUCCESS;
}


int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  
  video_graphic_init(0x14C);

  xpm_image_t xpm_img;
  xpm_map_t m = benfica_1908;
  uint8_t *map = xpm_load(m, XPM_8_8_8_8 , &xpm_img);

  if (map == NULL)
  {
    return EXIT_FAILURE;
  }
  
  uint16_t img_height = xpm_img.height;
  uint16_t img_width = xpm_img.width;

  for (unsigned int height = 0 ; height < img_height ; height++) {
    for (unsigned int width = 0 ; width < img_width; width++) {

      RGB rgb = RGB_new(0);

      rgb.setBlue(&rgb,*map);
      map++;
      rgb.setGreen(&rgb,*map);
      map++;
      rgb.setRed(&rgb,*map);
      map++;
      map++;

      fill_pixel(x+width,y+height, rgb.value);
    }
  }

  uint8_t kbc_bit_no = 1;
  int kbc_hook_id = 0, ipc_status;
  bool esc_pressed = false, r;
  uint16_t irq_set = BIT(kbc_bit_no);

  message msg;
  
  unsigned char scan[2];
  int scan_size;

  CHECKCall(subscribe_kbc_interrupt(kbc_bit_no, &kbc_hook_id));

  while (!esc_pressed) { 

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) { 
            kbc_ih();
            if (!kbc_get_error()) {
              if (kbc_ready()) {
                kbc_get_scancode(scan, &scan_size);
                if (scan[scan_size - 1] == ESC_BREAK_CODE) {
                  esc_pressed = true;
                }
              }
            }
          }
          break;
        default:
          break;
      }
    }
    else {
    }
  }

  CHECKCall(unsubscribe_interrupt(&kbc_hook_id));
  CHECKCall(vg_exit());

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
