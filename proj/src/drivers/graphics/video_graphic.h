#ifndef VIDEO_GRAPHIC_H
#define VIDEO_GRAPHIC_H

#include <lcom/lcf.h>

#include "vbe.h"
#include "../utils/handlers.h"

/**
 * TODO :::: make this more OOP 
 * 
 */

static void *video_mem;         /* frame-buffer VM address 
				   (static global variable*/

static void *buf;

static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static uint8_t color_model; // TODO: HOW TO USE THIS INSTEAD OF THE OTHER ONE

static unsigned int vram_size;

static uint8_t RedMaskSize;
static uint8_t GreenMaskSize;
static uint8_t BlueMaskSize;

static uint8_t RedFieldPosition;
static uint8_t GreenFieldPosition;
static uint8_t BlueFieldPosition;

int (vbe_set_mode)(uint16_t mode);
int (map_vram)(uint16_t mode);
void *(video_graphic_init)(uint16_t mode);
int (flush_screen)();

int(fill_pixel)(uint16_t x, uint16_t y, uint32_t color);
int(fill_pixel_transp)(uint16_t x, uint16_t y, uint32_t color);
int(vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

bool (is_indexed_mode)(uint16_t mode);
unsigned (get_hres)(void);
unsigned (get_vres)(void);
unsigned (get_bits_per_pixel)(void);

uint8_t (getRedMaskSize)(void);
uint8_t (getGreenMaskSize)(void);
uint8_t (getBlueMaskSize)(void);

uint8_t (getRedFieldPosition)(void);
uint8_t (getGreenFieldPosition)(void);
uint8_t (getBlueFieldPosition)(void);


#endif
