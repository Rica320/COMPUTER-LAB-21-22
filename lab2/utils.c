#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  /* To be implemented by the students */
  if(lsb==NULL) return 1;
  *lsb = (uint8_t) val;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  /* To be implemented by the students */
  if(msb==NULL) return 1;
  *msb = (uint8_t) (val >> 8);
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  /* To be implemented by the students */
  uint32_t new_val;
  
  if(sys_inb(port, &new_val)!=0){
    printf("Error in util_sys_inb\n");
    return 1;
  }
  *value=new_val & 0xFF;
  
  return 0;
}
