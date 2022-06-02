#include "rtc.h"

uint8_t rtc_data[6]; // sec, min, hour, day, month, year
static int rtc_hook_id;

// inibir updates é igual so que com RTC_SET
int enable_int_update(bool enable) {
  uint32_t reg;

  sys_outb(RTC_ADDR_REG, RTC_REG_B);
  sys_inb(RTC_DATA_REG, &reg);

  if (enable)
    reg |= RTC_UIE;
  else
    reg &= ~RTC_UIE;

  sys_outb(RTC_ADDR_REG, RTC_REG_B);
  sys_outb(RTC_DATA_REG, reg);

  return 0;
}

int(rtc_subscribe_int)(uint8_t *bit_no) {
  *bit_no = rtc_hook_id = RTC_IRQ;
  sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &rtc_hook_id);
  enable_int_update(true);
  return 0;
}

int(rtc_unsubscribe_int)() {
  sys_irqrmpolicy(&rtc_hook_id);
  enable_int_update(false);
  return 0;
}

void(rtc_ih)() {
  uint32_t reg;

  sys_outb(RTC_ADDR_REG, RTC_REG_C);
  sys_inb(RTC_DATA_REG, &reg);

  // atualizar os valores todos
  // caso int seja do tipo Update
  if (RTC_UF & reg) {
    rtc_read(RTC_SEC, 0);
    rtc_read(RTC_MIN, 1);
    rtc_read(RTC_HOURS, 2);
    rtc_read(RTC_DAY, 3);
    rtc_read(RTC_MONTH, 4);
    rtc_read(RTC_YEAR, 5);
  }
}

// esperar que rtc atualize os seus valores
int wait_rtc() {
  uint32_t reg = 0;

  // prof recomenda desativar e reativar ints
  // (por causa de preemption)
  do {

    sys_irqdisable(&rtc_hook_id);
    sys_outb(RTC_ADDR_REG, RTC_REG_A);
    sys_inb(RTC_DATA_REG, &reg);
    sys_irqenable(&rtc_hook_id);

  } while (RTC_UIP & reg);

  return 0;
}

// converte bcd --> decimal (pode dar problemas?)
uint8_t bcd2dec(uint8_t bcdNum) {
  return (bcdNum - 6 * (bcdNum >> 4));
}

// le valor do campo de argumento e atualiza valor na var global
int rtc_read(uint8_t val, uint8_t pos) {

  // esperar caso o rtc esteja a atualizar valores
  wait_rtc();

  // ler o campo desejado
  uint32_t read;
  sys_outb(RTC_ADDR_REG, val);
  sys_inb(RTC_DATA_REG, &read);

  // converter bcd --> decimal
  // guardar valor no campo certo
  rtc_data[pos] = bcd2dec(read);
  return 0;
}
