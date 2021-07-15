/* rtc.h - Defines used in the rtc
 * vim:ts=4 noexpandtab
 */

#ifndef _RTC_H
#define _RTC_H


#include "i8259.h"
#include "lib.h"
#include "types.h"

// constants defined for RTC ports and registers
#define regA  0x0A
#define regB  0x8B
#define regC  0x0C
#define RTC_PORT_1  0x70
#define RTC_PORT_2  0x71
#define RTC_IRQ  0x08
#define bit_mask 0x40
#define bit_mask_2 0xF0
#define FREQ_2  2
#define FREQ_4  4
#define FREQ_8  8
#define FREQ_16 16
#define FREQ_32 32
#define FREQ_64 64
#define FREQ_128  128
#define FREQ_256  256
#define FREQ_512  512
#define FREQ_1024 1024
#define FREQ_2_VAL 0x0F
#define FREQ_4_VAL 0x0E
#define FREQ_8_VAL 0x0D
#define FREQ_16_VAL 0x0C
#define FREQ_32_VAL 0x0B
#define FREQ_64_VAL 0x0A
#define FREQ_128_VAL 0x09
#define FREQ_256_VAL 0x08
#define FREQ_512_VAL 0x07
#define FREQ_1024_VAL 0x06

/* Initializes the RTC */
void rtc_init();

/* Executes RTC interrupt handler */
void rtc_handler();

/* Opens RTC */
extern int32_t rtc_open(const uint8_t* filename);

/* Reads RTC and changes RTC interrupt frequency to 2 Hz */
extern int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes);

/* Writes to RTC by setting frequency to that specified by buffer */
extern int32_t rtc_write(int32_t fd,const void* buf, int32_t nbytes);

/* Closes RTC */
extern int32_t rtc_close(int32_t fd);

#endif /* _RTC_H */
