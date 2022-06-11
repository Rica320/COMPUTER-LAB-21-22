#include <lcom/lcf.h>

#define RTC_IRQ 8

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

// Valores de acesso
#define RTC_SEC 0
#define RTC_SEC_A 1
#define RTC_MIN 2
#define RTC_MIN_A 3
#define RTC_HOURS 4
#define RTC_HOURS_A 5
#define RTC_WEEK_DAY 6
#define RTC_DAY 7
#define RTC_MONTH 8
#define RTC_YEAR 9

// Reg
#define RTC_REG_A 10
#define RTC_REG_B 11
#define RTC_REG_C 12
#define RTC_REG_D 13

// Configuração
#define RTC_UIP BIT(7)
#define RTC_SET BIT(7)
#define RTC_UIE BIT(4)

// Interrupções
#define RTC_UF BIT(4) // Update Interrupt Pending
#define RTC_AF BIT(5) // Alarm Interrupt Pending
#define RTC_PF BIT(6) // Periodic Interrupt Pending

/**
 * @brief Enables/Disables Interrupt Updates for the RTC
 * @param enable True for enabling, False for disabling
 * @return 0 on success
 */
int(enable_int_update)(bool enable);

/**
 * @brief Subscribes RTC Interrupts
 * @param bit_no Identifier of the interrupt to be subscribed
 * @return 0 on success
 */
int(rtc_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes RTC Interrupts
 * @return 0 on success
 */
int(rtc_unsubscribe_int)();

/**
 * @brief Handles RTC Interrupts
 */
void(rtc_ih)();

/**
 * @brief Waits for the RTC to update its values
 * @return 0 on success
 */
int(wait_rtc)();

/**
 * @brief Converts Binary Coded Decimal to Decimal
 * @param bcdNum Value expressed in Binary Coded Decimal
 * @return bcdNum value expressed in Decimal
 */
uint8_t(bcd2dec)(uint8_t bcdNum);

/**
 * @brief Reads Value from Argument Field and Stores it in the global variable
 * @return 0 on success
 */
int(rtc_read)(uint8_t reg, uint8_t pos);
