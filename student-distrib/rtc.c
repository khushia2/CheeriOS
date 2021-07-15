#include "rtc.h"
#include "lib.h"
#include "i8259.h"

volatile int interrupt_flag = 0; // interrupt flag that keeps track of whether an interrupt has occurred

/* rtc_init
 *
 * Initializes the RTC
 * Inputs: None
 * Outputs: None
 * Side Effects: enables the irq associated with rtc
 */
void rtc_init() {
    char prev;
    cli();
    disable_irq(RTC_IRQ); // disable rtc irq
    outb(regB, RTC_PORT_1);	 // select Register B, and disable NMI
    prev = inb(RTC_PORT_2);  // get current value of reg B
    outb(regB, RTC_PORT_1);	 // set index
    outb(prev | bit_mask, RTC_PORT_2);  //turn ON bit 6 of reg B by ORing with 0x40 and enable rtc_irq
    sti();
    enable_irq(RTC_IRQ);
}


/* rtc_handler
 *
 * RTC interrupt handler
 * Inputs: None
 * Outputs: None
 * Side Effects: sends EOI signal
 */
void rtc_handler() {
    //test_interrupts();
  send_eoi(RTC_IRQ);
  cli();
  interrupt_flag = 1; //interrupt has occurred so flag set to 1
  outb(regC, RTC_PORT_1);   // read from register C else interrupt wont happen again
  inb(RTC_PORT_2);
  sti();
}


/* rtc_open
 *
 * RTC function to open rtc and set periodic interrupt frequency to 2Hz.
 * Inputs: None
 * Outputs: None
 * Side Effects: Sets the frequency of RTC periodic interrupts to 2 Hz.
 */
int32_t rtc_open(const uint8_t* filename){
  cli();
  outb(regA, RTC_PORT_1); // select Register B and disable NMI
  unsigned char c = inb(RTC_PORT_2); // get current value of reg B
  outb(regA, RTC_PORT_1); // set index
  outb((bit_mask_2 & c) | (FREQ_2_VAL & FREQ_2_VAL), RTC_PORT_2); // set frequency value to 2 Hz.
  sti();
  return 0;
}


/* rtc_read
 *
 * RTC function to read rtc and wait for one interrupt to occur
 * Inputs: None
 * Outputs: None
 * Side Effects: clears interrupt flag after an interrupt has occurred
 */
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes){
  while (interrupt_flag == 0){} // wait for interrupt to occur
  interrupt_flag = 0; // clear when one interrupt has occurred
  return 0;
}


/* rtc_write
 *
 * RTC function to write rtc and set periodic interrupt frequency to that specified by buffer.
 * Inputs: None
 * Outputs: None
 * Side Effects: Sets the frequency of RTC periodic interrupts to that specified by buffer.
 */
int32_t rtc_write(int32_t fd,const void* buf, int32_t nbytes){
  unsigned char val;
  uint32_t rate;
  if(nbytes != FREQ_4){return -1;} // checks if number of bytes is 4 or not
  if(buf == NULL){return -1;} // checks if the pointer to buffer is valid or not
  rate = *(uint32_t *)buf; // gets frequency rate by dereferencing buffer
  if((rate&(rate-1)) != 0){return -1;} // checks if rate is a power of two or not
    /* checks which frequency and assigns the correct value to val variable */
  if(rate == FREQ_1024){val = FREQ_1024_VAL;}
  else if(rate == FREQ_512){val = FREQ_512_VAL;}
  else if(rate == FREQ_256){val = FREQ_256_VAL;}
  else if(rate == FREQ_128){val = FREQ_128_VAL;}
  else if(rate == FREQ_64){val = FREQ_64_VAL;}
  else if(rate == FREQ_32){val = FREQ_32_VAL;}
  else if(rate == FREQ_16){val = FREQ_16_VAL;}
  else if(rate == FREQ_8){val = FREQ_8_VAL;}
  else if(rate == FREQ_4){val = FREQ_4_VAL;}
  else if(rate == FREQ_2){val = FREQ_2_VAL;}
  else{return -1;}
  cli();
  /* sets frequency of intterupts to that specified by buffer */
  outb(regA, RTC_PORT_1);
  unsigned char c = inb(RTC_PORT_2);
  val &= FREQ_2_VAL;
  outb(regA, RTC_PORT_1);
  outb((bit_mask_2 & c) | val, RTC_PORT_2);
  sti();
  return 0;
}


/* rtc_close
 *
 * RTC function to open rtc. It does nothing.
 * Inputs: None
 * Outputs: None
 * Side Effects: None
 */
int32_t rtc_close(int32_t fd){
  /* does nothing */
  return 0;
}
