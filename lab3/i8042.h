#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>


/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard.
 */


#define DELAY_US    20000

/* KBC IRQ */

#define KBC_IRQ  1

/* I/O Ports Addresses */

#define KBC_CMD_REG  0x64 
#define KBC_ST_REG  0x64 

#define OUT_BUF 0x60

/* Status Register MASKS  */

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


/* KBC Commands */

#define KBC_READ_CMD          0x20 
#define KBC_WRITE_CMD         0x60 
#define KBC_CHECK             0xAA 
#define KBC_INTERFACE_TEST    0xAB 
#define KBC_INTERFACE_DISABLE 0xAD 
#define KBC_INTERFACE_ENABLE  0xAE 

/* Command byte masks */

#define INT_KBD              BIT(0)  
#define INT_MOUSE            BIT(1)  
#define DIS_KBD              BIT(4)  
#define DIS_MOUSE            BIT(5)  

#endif
