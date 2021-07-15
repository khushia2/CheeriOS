/* keyboard.h - Defines for handling keyboard operations
 */
#include "types.h"

#define KEYBOARD_NUM 0x01
#define KEYBOARD_PORT 0x60
#define NUM_CHARS 95
#define NUM_SCAN 48
#define UPPERCASEOFFSET 48
#define CRTLCHECK 0x26
#define ENTERPRESSED 0x1C
#define BACKSPACEPRESSED 0x0E
#define TABPRESSED 0x0F
#define SPACEPRESSED 0x39
#define CAPSLOCK 0x3A
#define LSHFTPRESSED 0x2A
#define RSHFTPRESSED 0x36
#define LSHFTDEPRESSED 0xAA
#define RSHFTDEPRESSED 0xB6
#define CRTLPRESSED 0x1D
#define CRTLDEPRESSED 0x9D
#define ALTPRESSED 0x38
#define ALTDEPRESSED 0xB8
#define BUFFERLIMIT 128
#define WRITELIMIT 127
#define VIDMEM 0xB8000
#define F1 0x3B
#define F2 0x3C
#define F3 0x3D
#define VIDIDX 184
#define KBPAGE 0x1000
#define COPYSIZE 4000
#define COL_COUNT 80


 /* initializes the keyboard */
extern void keyboard_init();

/* the keyboard handler */
extern void keyboard_handler();

/* open the keyboard */
extern int32_t open_terminal(const uint8_t* filename);

/* closes the keyboard */
extern int32_t close_terminal(int32_t fd);

/* reads from the keyboard into buffer */
extern int32_t read_terminal(int32_t fd, void* buf, int32_t nbytes);

/* writes from keyboard to screen */
extern int32_t write_terminal(int32_t fd, const void* buf, int32_t nbytes);

/* returns the terminal number that we are currently on */
extern int get_cur_terminal();
