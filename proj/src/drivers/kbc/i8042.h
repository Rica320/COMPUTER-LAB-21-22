/**
 * @file i8042.h
 * @brief Contains useful macros for use with the i0842 Keyboard.
 * @version 0.1
 * @date 2022-05-30
 */

#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 Keyboard.
 */

#define DELAY_US 20000

/* KBC IRQ */
// TODO: CHANGE THE NAME BELOW
/**
 * \def KBC_IRQ
 * \brief KBC IRQ Line
 */
#define KBC_IRQ 1
/**
 * \def MOUSE_IRQ
 * \brief Mouse IRQ Line
 */
#define MOUSE_IRQ 12

/* I/O Ports Addresses */

/**
 * \def KBC_CMD_REG
 * \brief KBC Command Register
 */
#define KBC_CMD_REG 0x64
/**
 * \def KBC_CMD_ARG
 * \brief Output Buffer where command arguments should be sent
 */
#define KBC_CMD_ARG 0x60
/**
 * \def KBC_ST_REG
 * \brief KBC Status Register
 */
#define KBC_ST_REG 0x64

/**
 * \def OUT_BUF
 * \brief KBC Output Buffer
 */
#define OUT_BUF 0x60
/**
 * \def IN_BUF
 * \brief KBC Input Buffer
 */
#define IN_BUF 0x60

/* Status Register MASKS  */

#define OUT_BUF_FULL BIT(0)
#define IN_BUF_FULL BIT(1)
#define SYS_FLAG BIT(2)
#define A2_INPUT_LINE BIT(3)
#define INH_FLAG BIT(4)
#define AUX_MOUSE BIT(5)
#define TIMEOUT_ERROR BIT(6)
#define PARITY_ERROR BIT(7)

/* Scancode */

#define ESC_BREAK_CODE 0x81
#define TWO_BYTE_CODE 0xE0
#define BREAK_CODE_BIT BIT(7)

/* KBC Commands */

#define KBC_READ_CMD 0x20
#define KBC_WRITE_CMD 0x60
#define KBC_CHECK 0xAA
#define KBC_INTERFACE_TEST 0xAB
#define KBC_INTERFACE_DISABLE 0xAD
#define KBC_INTERFACE_ENABLE 0xAE

/* KBC SELF CHECK STATUS */

#define KBC_CHECK_OK 0x55
#define KBC_CHECK_ERROR 0xFC

/* Command byte masks */

#define INT_KBD BIT(0)
#define INT_MOUSE BIT(1)
#define DIS_KBD BIT(4)
#define DIS_MOUSE BIT(5)

/*MOUSE first byte info*/

#define LB_MOUSE BIT(0)
#define RB_MOUSE BIT(1)
#define MB_MOUSE BIT(2)
#define BYTE1_VALIDATOR BIT(3)
#define MSB_X_DELTA BIT(4)
#define MSB_Y_DELTA BIT(5)
#define X_OVERFLOW BIT(6)
#define Y_OVERFLOW BIT(7)

/* MOUSE KBC CMDS*/

#define DIS_MOUSE_CMD 0xA7
#define ENA_MOUSE_CMD 0xA8
#define CHECK_MOUSE_INTERFACE 0xA8
#define WRITE_BYTE_MOUSE 0xD4
#define DIS_DATA_REP 0xF5
#define ENA_DATA_REP 0xF4

/* Mouse response*/
#define ACK 0xFA
#define NACK 0xFE

#endif
