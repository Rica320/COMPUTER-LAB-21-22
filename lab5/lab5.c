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
#include "sprite.h"
#include "time.h"
#include "i8254.h"

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

  flush_screen();


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

  vg_draw_rectangle(0, 0, get_hres(), get_vres(), 0x00ff00ff);
  
  
  xpm_map_t m = minix3_xpm;
  sprite_t * minix = make_sprite(m , XPM_8_8_8_8);
  
  set_sprite_X(minix, x);
  set_sprite_Y(minix, y);

  draw_sprite_in_mode_14c(minix);

  flush_screen();

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

  free_sprite(minix);

  CHECKCall(unsubscribe_interrupt(&kbc_hook_id));
  CHECKCall(vg_exit());

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  
  video_graphic_init(0x105);
  

  sprite_t * sprite = make_sprite(xpm , XPM_INDEXED);
  
  set_sprite_X(sprite, xi);
  set_sprite_Y(sprite, yi);

  uint8_t kbc_bit_no = 1;
  int kbc_hook_id = 0, ipc_status;
  bool esc_pressed = false, r;
  uint16_t irq_kbc_set = BIT(kbc_bit_no);

  message msg;

  unsigned char scan[2];
  int scan_size;

  CHECKCall(subscribe_kbc_interrupt(kbc_bit_no, &kbc_hook_id));
  
  uint8_t timer_id = 2; // THE WAY WE IMPLEMENTED WE ALREADY KNOW THE TIMER ID
  uint16_t irq_timer_set; // TODO: 16 bits ?
  CHECKCall(timer_subscribe_int(&timer_id)); 
  irq_timer_set = BIT(timer_id);

  int counter = 0;
  int frames = 0;
  int ticks_frame = TIMER_ASEC_FREQ / fr_rate;

  bool move_right = xf>xi;
  bool move_down = yf>yi;

  int mov = 1;
  if (speed > 0)
    mov = speed;

  while (!esc_pressed) {

    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_kbc_set) {
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
          if (msg.m_notify.interrupts & irq_timer_set) { 
            counter++;
            frames++;
            if (counter % ticks_frame == 0) {
              vg_draw_rectangle(0, 0, get_hres(), get_vres(), 0x0);
          
              if (mov == 1)
              {
                frames++;
                if (frames % (-speed) != 0)
                  break;
              }
              
              
              if (xi != xf) {
                if (move_right) {
                  if (mov + get_sprite_X(sprite) > xf)
                  {
                    set_sprite_X(sprite, xf);
                  } else {
                    set_sprite_X(sprite, get_sprite_X(sprite) + mov);
                  }
                } else {
                  if (get_sprite_X(sprite) - mov < xf)
                  {
                    set_sprite_X(sprite, xf);
                  } else {
                    set_sprite_X(sprite, get_sprite_X(sprite) - mov);
                  }
                }
              }
              if (yi != yf) {
                if (move_down) {
                  if (mov + get_sprite_Y(sprite) > yf)
                  {
                    set_sprite_Y(sprite, yf);
                  } else {
                    set_sprite_Y(sprite, get_sprite_Y(sprite) + mov);
                  }
                } else {
                  if (get_sprite_Y(sprite) - mov < yf)
                  {
                    set_sprite_Y(sprite, yf);
                  } else {
                    set_sprite_Y(sprite, get_sprite_Y(sprite) - mov);
                  }
                }
              }
              
            }
            draw_sprite_in_mode_105(sprite);
            flush_screen();
          }
          break;
        default:
          break;
      }
    }
    else {
    }
  }

  free_sprite(sprite);

  CHECKCall(timer_unsubscribe_int());
  CHECKCall(unsubscribe_interrupt(&kbc_hook_id));
  CHECKCall(vg_exit());


  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
