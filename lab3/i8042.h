#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>


/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard.
 */

#define KBC_IRQ  1


#define OUT_BUF 0x60

#define KBC_WR_CMD     0x64 
#define KBC_ST_REG  0x64 

#define OUTPUT_BUF  0x60



/* Status Register */

#define OUT_BUF_FULL    BIT(0) 
#define IN_BUF_FULL     BIT(1) 
#define SYS_FLAG        BIT(2) 
#define A2_INPUT_LINE   BIT(3) 
#define INH_FLAG        BIT(4) 
#define AUX_MOUSE       BIT(5) 
#define TIMEOUT_ERROR   BIT(6) 
#define PARITY_ERROR    BIT(7) 


/* Scancode */

#define ESC_BREAK_CODE  0x81
#define TWO_BYTE_CODE   0xE0
#define BREAK_CODE_BIT  BIT(7)  



#endif
