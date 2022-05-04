#ifndef _LCOM_I8042_H
#define _LCOM_I8042_H
#include <lcom/lcf.h>

// MACROS FOR KEYBOARD

/*         STATUS REGISTER

  Bit Name Meaning (if set)
  7 Parity Parity error - invalid data
  6 Timeout Timeout error - invalid data
  5 Aux Mouse data
  4 INH Inhibit flag: 0 if keyboard is inhibited
  3 A2 A2 input line: irrelevant for LCOM
  2 SYS System flag: irrelevant for LCOM
  1 IBF Input buffer full
  don’t write commands or arguments
  0 OBF Output buffer full - data available for reading

*/

// Errors
#define SR_PARITY_ERROR BIT(7)
#define SR_TO_ERROR BIT(6)
#define OBF BIT(0)
#define IBF BIT(1)

#define KBD_IRQ 1
#define DELAY_US 20000

#define ESC_BREAKCODE 0x81
#define MSB_2B_SCANCODE 0xE0 // scancodes with 2B have this msb

// Ports
#define REG_PORT 0x64 // Status Register / KBC commands
#define BUF_PORT 0x60 // In/Out buffer

// KBC Commands
#define KBC_READ_CMD 0x20
#define KBC_WRITE_CMD 0x60
#define KBC_CHECK 0xAA
#define KBC_INTERFACE_TEST 0xAB
#define KBC_INTERFACE_DISABLE 0xAD
#define KBC_INTERFACE_ENABLE 0xAE

// Command byte masks
#define INT_KBD BIT(0)
#define INT_MOUSE BIT(1)
#define DIS_KBD BIT(4)
#define DIS_MOUSE BIT(5)

#endif /* _LCOM_I8042_H */
