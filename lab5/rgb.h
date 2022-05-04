#ifndef RGB_H
#define RGB_H

#include <lcom/lcf.h>

#include "vc.h"

typedef struct RGB {
  uint32_t value;

  uint8_t (*getRed)(struct RGB *);
  uint8_t (*getGreen)(struct RGB *);
  uint8_t (*getBlue)(struct RGB *);
  uint8_t (*getTransparent)(struct RGB *);

  void (*setRed)(struct RGB *, uint8_t value);
  void (*setGreen)(struct RGB *, uint8_t value);
  void (*setBlue)(struct RGB *, uint8_t value);
  void (*setTransparency)(struct RGB *, uint8_t value);

} RGB, *pno;

void put_references(RGB *rgb);

RGB RGB_new(uint32_t color);

uint8_t getRed_t(struct RGB *self);
uint8_t getGreen_t(struct RGB *self);
uint8_t getBlue_t(struct RGB *self);
uint8_t getTransparent_t(struct RGB *self);

void setRed_t(struct RGB *self, uint8_t value);
void setGreen_t(struct RGB *self, uint8_t value);
void setBlue_t(struct RGB *self, uint8_t value);
void setTransparency_t(struct RGB *, uint8_t value);

typedef union {
  RGB rgb;
  uint32_t value;
  uint32_t array[4];
} Color;

#endif
