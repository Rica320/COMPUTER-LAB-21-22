#ifndef _LCOM_EVENT_H_
#define _LCOM_EVENT_H_

#include <lcom/lcf.h>


enum EVENT {NO_EVT, TIMER_EVT, KBD_EVT, MOUSE_EVT, BREAK_EVT};
typedef enum EVENT EVENT_T;

typedef uint8_t EVENTS;

#endif
