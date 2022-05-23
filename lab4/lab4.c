// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "mouse.h"
#include "timer.h"
#include "utils.h"

extern int counter;
extern bool full_packet;
extern uint8_t packet[3];

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  //lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  //lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(mouse_test_packet)(uint32_t cnt) {
  mouse_config();
  printf("Initializing...\n");
  int ipc_status, hook_id = 1;
  bool r;
  message msg;
  printf("Done!\nSubscribing Interrupts...\n");
  mouse_subscribe_int(&hook_id);
  printf("Done!\nStarting The Interrupt Loop...\n");
  while( cnt > 0 ) {
    printf("Loop Start...\n");
  if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
   printf("driver_receive failed with: %d", r);
   continue;
  }
  printf("driver_receive sucessful...\n");
  if (is_ipc_notify(ipc_status)) { // received notification
    printf("Interrupt Notification Received...\n");
    switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: //hardware interrupt notification                
        if (msg.m_notify.interrupts & BIT(hook_id)) { //subscribed interrupt
                printf("Handling the Mouse Interrupt...\n");
                mouse_ih();
                printf("Done!\nChecking if packet is complete...\n");
          if(full_packet){
            printf("Packet is complete!\nParsing and printing the packet...\n");
            full_packet = false;
            mouse_parse_packet(packet);
            cnt--;
          }
        }
        break;   
      default:
       break; // no other notifications expected: do nothing    
    }
 } else { // received a standard message, not a notification
    printf("Not notified for interrupts...\n");
       // no standard messages expected: do nothing
   }
}
  printf("All interrupts dealt with... Unsubscribing...\n");
  mouse_unsubscribe_int(&hook_id);
  printf("Done!\nReconfiguring Mouse...\n");
  mouse_reconfig();
  printf("Done!\nTerminating...\n");
  return 0;
}

int(mouse_test_async)(uint8_t idle_time) {
  mouse_config();
  printf("Initializing...\n");
  uint8_t idletime = idle_time;
  int ipc_status, mouse_id = 1, timer_id = 2;
  bool r;
  message msg;
  printf("Done!\nSubscribing Interrupts...\n");
  mouse_subscribe_int(&mouse_id);
  timer_subscribe(&timer_id);
  printf("Done!\nStarting The Interrupt Loop...\n");
  while(idletime) {
    printf("Loop Start...\n");
  if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
   printf("driver_receive failed with: %d", r);
   continue;
  }
  printf("driver_receive sucessful...\n");
  if (is_ipc_notify(ipc_status)) { // received notification
    printf("Interrupt Notification Received...\n");
    switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE: //hardware interrupt notification                
        if (msg.m_notify.interrupts & BIT(mouse_id)) { //if the notification is mouse interrupt
          printf("Handling the Mouse Interrupt...\n");
          mouse_ih();
          printf("Done!\nChecking if packet is complete...\n");     
          if(full_packet){
            printf("Packet is complete!\nParsing and printing the packet...\n");
            full_packet = false;
            mouse_parse_packet(packet);
          }
        }
        if (msg.m_notify.interrupts & BIT(timer_id)){
          printf("Handling the Timer Interrupt...\n");
          timer_int_handler();
          if(counter % 60 == 0){
            idletime--;
          }
        }
        break;   
      default:
       break; // no other notifications expected: do nothing    
    }
 } else { // received a standard message, not a notification
    printf("Not notified for interrupts...\n");
       // no standard messages expected: do nothing
   }
}
  printf("Idle time exceeded... Unsubscribing...\n");
  mouse_unsubscribe_int();
  timer_unsubscribe_int();
  printf("Done!\nReconfiguring Mouse...\n");
  mouse_reconfig();
  printf("Done!\nTerminating...\n");
  return 0;
}

int(mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  return 1;
}

int(mouse_test_remote)(uint16_t period, uint8_t cnt) {
  //Não é para implementar
  return 0;
}
