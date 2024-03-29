#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

/** @defgroup i8254 i8254
 * @{
 *
 * Constants for programming the i8254 Timer. Needs to be completed.
 */

#define TIMER_FREQ 1193182 /**< @brief clock frequency for timer in PC and AT */
#define TIMER_ASEC_FREQ 60
#define TIMER_BB_FREQ 19 /**< @brief Hardware limitaion, it is 18.2hz */
#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */

#define TIMER0_HOOK_ID 2 /* just a made up assignment*/

/* AVAILABLE TIMERS */

#define FIRST_TIMER 0
#define SECOND_TIMER 1
#define THIRD_TIMER 2

/* I/O port addresses */

#define TIMER_0    0x40 /**< @brief Timer 0 count register */
#define TIMER_1    0x41 /**< @brief Timer 1 count register */
#define TIMER_2    0x42 /**< @brief Timer 2 count register */
#define TIMER_CTRL 0x43 /**< @brief Control register */

#define SPEAKER_CTRL 0x61 /**< @brief Register for speaker control  */

/* Timer control */

/* Timer selection: bits 7 and 6 */

#define TIMER_SEL0   0x00              /**< @brief Control Word for Timer 0 */
#define TIMER_SEL1   BIT(6)            /**< @brief Control Word for Timer 1 */
#define TIMER_SEL2   BIT(7)            /**< @brief Control Word for Timer 2 */
#define TIMER_RB_CMD (BIT(7) | BIT(6)) /**< @brief Read Back Command */
#define TIMER_SEL(n) ((n == 0) ? TIMER_SEL0 : BIT( 5 + (n) ))

/* Register selection: bits 5 and 4 */

#define TIMER_LSB     BIT(4)                  /**< @brief Initialize Counter LSB only */
#define TIMER_MSB     BIT(5)                  /**< @brief Initialize Counter MSB only */
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB) /**< @brief Initialize LSB first and MSB afterwards */

/* Operating mode: bits 3, 2 and 1 */

#define TIMER_SQR_WAVE (BIT(2) | BIT(1)) /**< @brief Mode 3: square wave generator */
#define TIMER_RATE_GEN BIT(2)            /**< @brief Mode 2: rate generator */

/* Counting mode: bit 0 */

#define TIMER_BCD 0x01 /**< @brief Count in BCD */
#define TIMER_BIN 0x00 /**< @brief Count in binary */

/* READ-BACK COMMAND FORMAT */

#define TIMER_RB_COUNT_  BIT(5)
#define TIMER_RB_STATUS_ BIT(4)
#define TIMER_RB_SEL(n)  BIT((n) + 1)

/* CONFIGURATION MODE */

#define CONF_IN_MODE(n) ((n & (BIT(5) | BIT(4))) >> 4)
#define CONF_COUNT_MODE(n) ((n & (BIT(3) | BIT(2) | BIT(1))) & BIT(2) ? \
                            ((n & (BIT(2) | BIT(1))) >> 1): ((n & (BIT(3) | BIT(2) | BIT(1))) >> 1))
#define CONF_BCD_MODE(n) (n & BIT(0))

/* SOME UTILS MACRO*/

#define LSHUB_IN_BYTE(n)  ( n & 0x0f ) /** less sig. half of a Unsigned byte bits (4 bits) */

/**@}*/


#endif /* _LCOM_I8254_H */
