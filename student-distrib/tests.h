#ifndef TESTS_H
#define TESTS_H

#define NUM_BYTES 4
#define LOOP_20 20
#define LOOP_50 50
#define LOOP_100 100
#define FREQ_2    2
#define FREQ_4    4
#define FREQ_8    8
#define FREQ_16    16
#define FREQ_32    32
#define FREQ_64    64
#define FREQ_128   128
#define FREQ_256   256
#define FREQ_512   512
#define FREQ_1024  1024
#define FREQ_GARB  2
#define LOOP_VAR  100000000
#define FREQ_32   32
#define VID_MEM   0xB8001
#define KERNEL_MEM 0x400001
#define NON_EX_MEM 0x01
#define NON_EX_PAGE 0x888888
#define MAXBUFFER 2048

// test launcher
void launch_tests();

#endif /* TESTS_H */
