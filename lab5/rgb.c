#include "rgb.h"

uint8_t getRed_t(struct RGB *self) {
  return (self->value >> getRedFieldPosition()) & ((1 << getRedMaskSize()) - 1);
}
uint8_t getGreen_t(struct RGB *self) {
  return (self->value >> getGreenFieldPosition()) & ((1 << getGreenMaskSize()) - 1);
}
uint8_t getBlue_t(struct RGB *self) {
  return (self->value >> getBlueFieldPosition()) & ((1 << getBlueMaskSize()) - 1);
}

uint8_t getTransparent_t(struct RGB *self) {
  return (self->value >> (getRedMaskSize() + getGreenMaskSize() + getBlueMaskSize()));
}

void setRed_t(struct RGB *self, uint8_t value) {

  uint32_t temp = (self->getGreen(self) << (getBlueMaskSize())) | self->getBlue(self);

  temp |= (value) << (getGreenMaskSize() + getBlueMaskSize());
  self->value = temp;
}
void setGreen_t(struct RGB *self, uint8_t value) {

  uint32_t temp = (self->getRed(self) << (getGreenMaskSize() + getBlueMaskSize())) | self->getBlue(self);

  temp |= ((value) << (getBlueMaskSize()));
  self->value = temp;
}
void setBlue_t(struct RGB *self, uint8_t value) {

  uint32_t temp = (self->getGreen(self) << (getBlueMaskSize())) | (self->getRed(self) << (getGreenMaskSize() + getBlueMaskSize()));

  temp |= (value);
  self->value = temp;
}

void setTransparency_t(struct RGB *self, uint8_t value) {

  unsigned maskSize = (getGreenMaskSize() + getBlueMaskSize() + getRedMaskSize());

  uint32_t temp = self->value;

  temp |= ((value) << (maskSize));

  self->value = temp;
}

void put_references(RGB *rgb) {
  rgb->getRed = getRed_t;
  rgb->getGreen = getGreen_t;
  rgb->getBlue = getBlue_t;
  rgb->getTransparent = getTransparent_t;

  rgb->setRed = setRed_t;
  rgb->setGreen = setGreen_t;
  rgb->setBlue = setBlue_t;
  rgb->setTransparency = setTransparency_t;
}

RGB RGB_new(uint32_t color) {
  RGB obj;

  obj.value = color;

  put_references(&obj);

  return obj;
}

