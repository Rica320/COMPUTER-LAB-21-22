#include "vc.h"

static void *video_mem; /* frame-buffer VM address(static global variable)*/

unsigned vram_base; /* VRAM's physical addresss */
unsigned vram_size; /* VRAM's size, but you can use the frame-buffer size, instead */

int r;

unsigned hres, vres;
unsigned numBytesPerPixel;

struct minix_mem_range mr, m2;
static mmap_t mem_map; // apontador para a memoria grafica FISICA --> é o q usamos pra mudar cores dos pixels
vbe_mode_info_t mode_info;

/*    FUNCTION FOR 1    */

int set_vc_mode(uint16_t mode) {
  return 0;
}

/*    FUNCTION FOR 2    */

int(set_graphics_card_mode)(uint16_t mode) {

  mem_map.phys = 0;
  mem_map.size = 1024 * 1024;
  lm_alloc(mem_map.size, &mem_map);

  /* Use VBE function 0x01 to initialize vram_base and vram_size */

  // usamos a func vbe 0x01 que ja nos e fornecida para poupar tempo
  if (vbe_get_mode_info(mode, &mode_info))
    return 1;

  numBytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
  vram_base = mode_info.PhysBasePtr;
  hres = mode_info.XResolution;
  vres = mode_info.YResolution;
  vram_size = hres * vres * numBytesPerPixel;

  RedMaskSize = mode_info.RedMaskSize;
  GreenMaskSize = mode_info.GreenMaskSize;
  BlueMaskSize = mode_info.BlueMaskSize;

  RedFieldPosition = mode_info.RedFieldPosition;
  GreenFieldPosition = mode_info.GreenFieldPosition;
  BlueFieldPosition = mode_info.BlueFieldPosition;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes) vram_base;
  mr.mr_limit = mr.mr_base + vram_size;

  if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vram_size);

  if (video_mem == MAP_FAILED)
    panic("couldn't map video memory");

  reg86_t r86;

  memset(&r86, 0, sizeof(reg86_t)); // clear struct to prevent irratic behaviour

  r86.ax = 0x4F02;
  r86.bx = BIT(14) | mode; // set bit 14: linear framebuffer
  r86.intno = 0x10;

  if (sys_int86(&r86) != OK) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
  }

  lm_free(&mem_map);

  return 0;
}

/*      Draw funcs used in 2 and project     */

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

uint8_t(getRedMaskSize)(void) {
  return RedMaskSize;
}
uint8_t(getGreenMaskSize)(void) {
  return GreenMaskSize;
}
uint8_t(getBlueMaskSize)(void) {
  return BlueMaskSize;
}

uint8_t(getRedFieldPosition)(void) {
  return RedFieldPosition;
}
uint8_t(getGreenFieldPosition)(void) {
  return GreenFieldPosition;
}
uint8_t(getBlueFieldPosition)(void) {
  return BlueFieldPosition;
}

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

int XPMmove(xpm_map_t xpm, MoveCords *cords, int16_t speed, uint8_t fr_rate) {

int cordsCalc(moveCords_t *cords, int16_t speed) {
  return 0;
}

/*  Auxiliar func */

  if (speed > 0) {
    // limpar posicao anterior
    vg_draw_rectangle(cords->newX, cords->newY, img.width, img.height, 0);
    cordsCalc(cords, speed);
  }

  else {
    if (frameCounter++ % abs(speed) == 0) {
      // limpar posicao anterior
      vg_draw_rectangle(cords->newX, cords->newY, img.width, img.height, 0);
      cordsCalc(cords, 1);
    }

    // if there is a message from an i/o
    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE)
      if (msg.m_notify.interrupts & BIT(bit_no)) {

        kbc_ih(); // chamar o nosso handler

int cordsCalc(MoveCords *cords, int16_t speed) {

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
