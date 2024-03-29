#ifndef _LCOM_EVENT_H_
#define _LCOM_EVENT_H_

#include <lcom/lcf.h>

/**
 * @brief Program Events
 * 
 */
enum EVENT { NO_EVT,
             TIMER_EVT,
             KBD_EVT,
             MOUSE_EVT,
             RTC_EVT,
             UART_EVT,
             BREAK_EVT };

typedef enum EVENT EVENT_T;

typedef uint16_t EVENTS;

#endif
