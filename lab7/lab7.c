#include <lcom/lcf.h>

#include "test7.h"
#include "stdio.h"
#include "uart.h"
#include "handlers.h"


int main(int argc, char **argv) {

	lcf_set_language("EN-US");

  	// enables to log function invocations that are being "wrapped" by LCF
  	// [comment this out if you don't want/need it]
  	lcf_trace_calls("/home/lcom/labs/lab7/trace.txt");

  	// enables to save the output of printf function calls on a file
  	// [comment this out if you don't want/need it]
  	lcf_log_output("/home/lcom/labs/lab7/output.txt");

  	// handles control over to LCF
  	// [LCF handles command line arguments and invokes the right function]
  	if (lcf_start(argc, argv))
  	  return 1;

  	// LCF clean up tasks
  	// [must be the last statement before return]
  	lcf_cleanup();


	return 0;
}

int(proj_main_loop)(int argc, char *argv[]){
	uint8_t lcr;

	CHECKCall(util_sys_inb( COM1_ADDR + UART_IER, &lcr));
	//printf("0x%08x \n", lcr);

	// we WILL HAVE TO DEVELOP SMTH TO INIT ... THERE SEEMS TO BE A DIFF BETWEEN THE STOP AND THE OTHER
	ser_test_set(COM1_ADDR, BIT(0) | BIT(1), 1, BIT(3), 115200);

	int r, ipc_status, hook_id;
	uint8_t bit_n;	
	ser_subscribe_int(&bit_n, &hook_id);
	set_ier(COM1_ADDR, IER_RECEIVED_INT | IER_RECEIVER_LINE_INT | IER_TRANSMITTER_INT, true);


	message msg;
	uint16_t irq_set = BIT(bit_n);
	int i = 0;

	sleep(2);
	uint8_t byte = 11;
	sys_outb(COM1_ADDR + UART_THR, byte);

	while (i < 1)
	{

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status))
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if (msg.m_notify.interrupts & irq_set)
				{
					printf("HELLO");
				}
				break;
			default:
				break;
			}
		}
		else
		{
		}
		i++;
	}

	ser_unsubscribe_int(&hook_id);

	return 0;
}
