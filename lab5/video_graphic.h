#ifndef VIDEO_GRAPHIC_H
#define VIDEO_GRAPHIC_H

#include <lcom/lcf.h>

#include <vbe.h>
#include <handlers.h>

/**
 * TODO :::: make this more OOP 
 * 
 */

static void *video_mem;         /* frame-buffer VM address 
				   (static global variable*/

static unsigned h_res;	        /* Horizontal resolution in pixels */
static unsigned v_res;	        /* Vertical resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static uint8_t color_model; // TODO: HOW TO USE THIS INSTEAD OF THE OTHER ONE

int (vbe_set_mode)(uint16_t mode);
int (map_vram)(uint16_t mode);
void *(video_graphic_init)(uint16_t mode);

bool (is_indexed_mode)(uint16_t mode);
unsigned (get_hres)(void);
unsigned (get_vres)(void);
unsigned (get_bits_per_pixel)(void);


#endif
