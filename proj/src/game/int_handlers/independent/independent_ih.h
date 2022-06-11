#ifndef _LCOM_INDEPENDENT_IH_H_
#define _LCOM_INDEPENDENT_IH_H_

#include "../../../drivers/kbc/i8042.h"
#include "../../../drivers/kbc/kbc.h"
#include "../../../drivers/mouse/mouse.h"
#include "../../../drivers/rtc/rtc.h"
#include "../../../drivers/serial_port/uart.h"
#include "../../../drivers/serial_port/uart_defines.h"
#include "../../../drivers/utils/handlers.h"
#include "../event.h"

static int ipc_status;
static message msg;
static bool r;

static uint16_t irq_set;
static uint16_t irq_mouse_set;
static uint16_t irq_timer;
static uint16_t irq_rtc;
static uint16_t irq_ser;

static int kbc_hook_id;
static int rtc_hook_id;
static int kbc_mouse_hook_id;
static int ser_hook_id;

void subscribe_ihs();
EVENTS handle_ihs();
void unsubscribe_ihs();

#endif
