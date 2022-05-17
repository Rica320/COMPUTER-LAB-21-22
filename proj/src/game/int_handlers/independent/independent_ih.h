#ifndef _LCOM_INDEPENDENT_IH_H_
#define _LCOM_INDEPENDENT_IH_H_

#include "../event.h"
#include "../../../drivers/kbc/i8042.h"
#include "../../../drivers/utils/handlers.h"
#include "../../../drivers/mouse/mouse.h"
#include "../../../drivers/kbc/kbc.h"

static int ipc_status;
static message msg;
static bool r;

static uint16_t irq_set;
static uint16_t irq_mouse_set;
static uint16_t irq_timer;

static int kbc_hook_id;     
static int kbc_mouse_hook_id;

void subscribe_ihs();
EVENT_T handle_ihs();
void unsubscribe_ihs();

#endif
