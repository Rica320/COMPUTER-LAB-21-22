#include <lcom/lcf.h>

#include "handlers.h"
#include <stdint.h>

/**
 * @brief Get the least significant byte of the given val
 *
 * @param val value to be checked
 * @param lsb lsb from the val
 * @return int 0 uppon Success
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

  NullSafety(lsb);
  *lsb = val;

  return EXIT_SUCCESS;
}

/**
 * @brief Get the most significant byte of the given val
 *
 * @param val value to be checked
 * @param msb msb from the val
 * @return int 0 uppon Success
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb) {

  NullSafety(msb);
  *msb = val >> 8;

  return EXIT_SUCCESS;
}

/**
 * @brief Write given value to the given port
 *
 * @param port where to write value
 * @param value value to be written
 * @return int 0 uppon Success
 */
int(util_sys_inb)(int port, uint8_t *value) {
  NullSafety(value);

  uint32_t p;
  CHECKCall(sys_inb(port, &p));

  *value = (uint8_t) p;

  return EXIT_SUCCESS;
}
