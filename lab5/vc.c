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

  /*  Interesting video modes
  0X105 --> 1024 x 768 with indexed color mode and colors of 1 byte

  0x115 --> 800 x 600 with direct color mode and colors of each pixel takes 24 bits, 8 for each of the RGB
  */

  reg86_t r; // struct to set values send to BIOS

  // clean the structure to avoid irratic behaviour
  memset(&r, 0, sizeof(r));

  r.intno = VIDEO_CARD;  // what to change
  r.ax = SET_VBE_MODE;   // VBE call function 02 --> set VBE Mode
  r.bx = BIT(14) | mode; // new value (bit 14 --> linear buffer)

  // Make the BIOS call
  if (sys_int86(&r)) {
    printf("\tvg_exit(): sys_int86() failed \n");
    return 1;
  }

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

  // criar pointer para a posição do pixel no array de mem (end base : video_mem)
  uint8_t *ptr;
  ptr = (uint8_t *) video_mem + (x * numBytesPerPixel) + (y * hres * numBytesPerPixel);
  // talvez uma funcao calculateMemPos fosse int para simplificar para o projeto

  for (unsigned i = 0; i < numBytesPerPixel; i++)
    ptr[i] = (color >> (i * 8)) & 0xFF;

  return 0;
}

int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned i = 0; i < len; i++)
    if (changePixelColor(x + i, y, color))
      return 1;
  return 0;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (unsigned i = 0; i < height; i++)
    if (vg_draw_hline(x, y + i, width, color))
      return 1;
  return 0;
}

int(vg_draw_fill)(uint32_t color) {

  for (unsigned i = 0; i < hres; i++)
    for (unsigned j = 0; j < vres; j++)
      if (changePixelColor(i, j, color))
        return 1;
  return 0;
}

/*    Functions for 3  */

int(video_pattern)(uint8_t no_rectangles, uint32_t first, uint8_t step) {

  uint16_t width = hres / no_rectangles;
  uint16_t height = vres / no_rectangles;

  for (unsigned h = 0; h < no_rectangles; h++) {

    // se esta no limite e n consegue desenhar um retangulo completo
    if (h * height >= (vres - (vres % no_rectangles)))
      break;

    for (unsigned w = 0; w < no_rectangles; w++) {

      // se esta no limite e n consegue desenhar um retangulo completo
      if (w * width >= (hres - (hres % no_rectangles)))
        break;

      if (mode_info.MemoryModel != DIRECT_COLOR) {
        uint32_t color = indexed_color(w, h, step, first, no_rectangles);
        vg_draw_rectangle(w * width, h * height, width, height, color);
      }
      else {
        uint32_t red = R(w, step, first);
        uint32_t green = G(h, step, first);
        uint32_t blue = B(w, h, step, first);
        uint32_t color = direct_color(red, green, blue);
        vg_draw_rectangle(w * width, h * height, width, height, color);
      }
    }
  }

  return 0;
}

/* Estas funcoes servem apenas para o LAB5 tendo em principio pouca utilidade para o trabalho  a n ser fazer gradientes */

uint32_t(indexed_color)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t no_rectangles) {
  return (first + (row * no_rectangles + col) * step) % (1 << mode_info.BitsPerPixel);
}

uint32_t(direct_color)(uint32_t red, uint32_t green, uint32_t blue) {
  return blue | (green << mode_info.GreenFieldPosition) | (red << mode_info.RedFieldPosition);
}

uint32_t(R_First)(uint32_t first) {
  return (first >> mode_info.RedFieldPosition) & ((1 << mode_info.RedMaskSize) - 1);
}

uint32_t(G_First)(uint32_t first) {
  return (first >> mode_info.GreenFieldPosition) & ((1 << mode_info.GreenMaskSize) - 1);
}

uint32_t(B_First)(uint32_t first) {
  return (first >> mode_info.BlueFieldPosition) & ((1 << mode_info.BlueMaskSize) - 1);
}

uint32_t(R)(unsigned w, uint8_t step, uint32_t first) {
  return (R_First(first) + w * step) % (1 << mode_info.RedMaskSize);
}

uint32_t(G)(unsigned h, uint8_t step, uint32_t first) {
  return (G_First(first) + h * step) % (1 << mode_info.GreenMaskSize);
}

uint32_t(B)(unsigned w, unsigned h, uint8_t step, uint32_t first) {
  return (B_First(first) + (w + h) * step) % (1 << mode_info.BlueMaskSize);
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

  int frameCounter = 0;
  xpm_image_t img;
  uint8_t *map;
  map = xpm_load(xpm, XPM_INDEXED, &img);

  // Atualiza valores das posicoes conforme speed e fr_rate

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
  }

  // desenhar xpm na nova posição
  map = xpm_load(xpm, XPM_INDEXED, &img);
  for (unsigned int height = 0; height < img.height; height++)
    for (unsigned int width = 0; width < img.width; width++)
      changePixelColor(cords->newX + width, cords->newY + height, *map++);

  return 0;
}

int cordsCalc(MoveCords *cords, int16_t speed) {

  if (cords->xi == cords->xf) {
    if (cords->yi < cords->yf) {
      if (cords->newY + speed > cords->yf)
        cords->newY = cords->yf;
      else
        cords->newY = cords->newY + speed;
    }
    else {
      if (cords->newY - speed < cords->yf)
        cords->newY = cords->yf;
      else
        cords->newY = cords->newY - speed;
    }
  }
  else {
    if (cords->xi < cords->xf) {
      if (cords->newX + speed > cords->xf)
        cords->newX = cords->xf;
      else
        cords->newX = cords->newX + speed;
    }
    else {
      if (cords->newX - speed < cords->xf)
        cords->newX = cords->xf;
      else
        cords->newX = cords->newX - speed;
    }
  }

  return 0;
}
