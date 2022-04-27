#include "rgb.h"

uint8_t getRed_t(struct RGB *self) {
  return ((self->value >> (getRedMaskSize() + getGreenMaskSize())) & ~(1 << getRedMaskSize())) ;
}
uint8_t getGreen_t(struct RGB *self) {
  return ((self->value) >> getGreenMaskSize())  & ~(1 << getGreenMaskSize()); 
}
uint8_t getBlue_t(struct RGB *self) {
  return (self->value & ~(1 << getBlueMaskSize()));
}

void setRed_t(struct RGB *self, uint8_t value) {

  uint32_t temp = (self->getGreen(self) << (getGreenMaskSize())) | self->getBlue(self);

  temp |= (value & ~(1 << getRedMaskSize())) << (getGreenMaskSize() + getBlueMaskSize());

  self->value = temp;
  return;
}
void setGreen_t(struct RGB *self, uint8_t value) {
  
  uint32_t temp = (self->getRed(self) << (getRedMaskSize())) | self->getBlue(self);

  temp |= (value & ~(1 << getGreenMaskSize())) << (getBlueMaskSize());

  self->value = temp;

  return ; 
}
void setBlue_t(struct RGB *self, uint8_t value) {
  
  uint32_t temp = (self->getGreen(self) << (getGreenMaskSize())) 
                | (self->getRed(self) << (getRedMaskSize())) ;

  temp |= (value & ~(1 << getBlueMaskSize()));

  self->value = temp;

  return;
}

void put_references(RGB * rgb) {
  rgb->getRed = getRed_t; 
  rgb->getGreen = getGreen_t; 
  rgb->getBlue = getBlue_t; 

  rgb->setRed = setRed_t; 
  rgb->setGreen = setGreen_t; 
  rgb->setBlue = setBlue_t; 
}

RGB RGB_new(uint32_t color) { 
  RGB obj;

  obj.value = color;

  put_references(&obj);

  return obj;
}

