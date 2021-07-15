/* keyboard.c - functions to enable the
 *              proper functioning of the keyboard
 *
 */
#include "keyboard.h"
#include "lib.h"
#include "i8259.h"
#include "x86_desc.h"

 /* array of the normal characters in order */
unsigned char normal_chars[NUM_CHARS] = { '1', '2', '3', '4', '5',
                                      '6', '7', '8', '9', '0',
                                      '-', '=', 'q', 'w', 'e',
                                      'r', 't', 'y', 'u', 'i',
                                      'o', 'p', '[', ']', 'a',
                                      's', 'd', 'f', 'g', 'h',
                                      'j', 'k', 'l', ';', '\'',
                                      '`', '\\', 'z', 'x', 'c',
                                      'v', 'b', 'n', 'm', ',',
                                      '.', '/',' ','!', '@',
                                      '#', '$', '%','^', '&',
                                      '*', '(', ')','_', '+',
                                      'Q', 'W', 'E','R', 'T',
                                      'Y', 'U', 'I','O', 'P',
                                      '{', '}', 'A','S', 'D',
                                      'F', 'G', 'H','J', 'K',
                                      'L', ':', '"','~', '|',
                                      'Z', 'X', 'C','V', 'B',
                                      'N', 'M', '<','>', '?'
};


/*
    'escape', 'backspace','tab','enter', 'control',
    'alt', 'l-shift', 'r-shift','capslock', 'F1',
    'F2', 'F3', 'F4','F5', 'F6',
    'F7', 'F8', 'F9','F10', 'F11',
    'F12', 'numlock', 'scrollock','delete', 'pause',
    'print', 'apps', 'gui','pup', 'pdown'

    0x01, 0x0E, 0x0F, 0x1C, 0x1D,
    0x38, 0x2A, 0x36, 0x3A, 0x3B,
    0x3C, 0x3D, 0x3E, 0x3F, 0x40,
    0x41, 0x42, 0x43, 0x44, 0x57,
    0x58, 0x45, 0x46, 0x53, 0xC5,
    0xB7, 0x5D, 0x5B, 0x49, 0x51
*/


/* scancodes corresponding to the order of the characters in normal_char and shift_chars */
unsigned char scan_pressed_normal[NUM_SCAN] = { 0x02,0x03,0x04,0x05,0x06,
                                           0x07,0x08,0x09,0x0A,0x0B,
                                           0x0C,0x0D,0x10,0x11,0x12,
                                           0x13,0x14,0x15,0x16,0x17,
                                           0x18,0x19,0x1A,0x1B,0x1E,
                                           0x1F,0x20,0x21,0x22,0x23,
                                           0x24,0x25,0x26,0x27,0x28,
                                           0x29,0x2B,0x2C,0x2D,0x2E,
                                           0x2F,0x30,0x31,0x32,0x33,
                                           0x34,0x35,0x39
};

//buffer variables
//char *globalbuffer;
char globalbuffer1[BUFFERLIMIT];
char globalbuffer2[BUFFERLIMIT];
char globalbuffer3[BUFFERLIMIT];
int* count;
int count1 = 0;
int count2 = 0;
int count3 = 0;
int maxflag = 0;
int readflag = 0;
int terminalCheck = 0;

//variables to keep track of special char
int capslockactive = 0;
int shiftpressed = 0;
int crtlpressed = 0;
int altpressed = 0;

//multiterm variable
extern int screen_x;
extern int screen_y;
extern int newlinecheck;
int screen_x1 = 0;
int screen_y1 = 0;
int newlinecheck1 = 0;
int screen_x2 = 0;
int screen_y2 = 0;
int newlinecheck2 = 0;
int screen_x3 = 0;
int screen_y3 = 0;
int newlinecheck3 = 0;

/* keyboard_init
 *
 * Initializes the keyboard
 * Inputs: None
 * Outputs: None
 * Side Effects: unmasks the pin corresponding to keyboard on the PIC
 */
void keyboard_init() {
    enable_irq(KEYBOARD_NUM);
    //globalbuffer = globalbuffer1;
    count = &count1;
    terminalCheck = 1;
}

/* keyboard_handler
 *
 * the handler for keyboard
 * Inputs: None
 * Outputs: None
 * Side Effects: echoes the pressed character
 */
void keyboard_handler() {
    send_eoi(KEYBOARD_NUM);
    unsigned char input_scan_code;
    input_scan_code = inb(KEYBOARD_PORT);
    if (*count >= (WRITELIMIT)) {
        maxflag = 1;
    }
    else {
        maxflag = 0;
    }
        //CRTL+L to clear screen
        if (crtlpressed == 1 && input_scan_code == CRTLCHECK) {
            //does not clear buffer
            clearscreen();
            return;
        }
        //Terminal 1
        if (altpressed == 1 && input_scan_code == F1) {
            if (terminalCheck == 1) {
                return;
            }
            //save current vidmemory
            void* vidpointer = (void*)VIDMEM;
            void* terminalpointer = (void*)((VIDIDX+terminalCheck) * KBPAGE);
            memcpy(terminalpointer, vidpointer, COPYSIZE);

            //saves current location on screen
            if (terminalCheck == 2) {
                screen_x2 = screen_x;
                screen_y2 = screen_y;
                newlinecheck2 = newlinecheck;
            }

            if (terminalCheck == 3) {
                screen_x3 = screen_x;
                screen_y3 = screen_y;
                newlinecheck3 = newlinecheck;
            }

            //globalbuffer = globalbuffer1;
            //set buffer variables to terminal we are switching to
            count = &count1;
            terminalCheck = 1;

            //set display video memory to terminal 1s vid mem
            screen_x = screen_x1;
            screen_y = screen_y1;
            newlinecheck = newlinecheck1;
            void* newterminalpointer = (void*)((VIDIDX + terminalCheck) * KBPAGE);
            memcpy(vidpointer, newterminalpointer, COPYSIZE);
            movingcursor(COL_COUNT * screen_y + screen_x);
            return;
        }
        //Terminal 2
        if (altpressed == 1 && input_scan_code == F2) {
            if (terminalCheck == 2) {
                return;
            }
            void* vidpointer = (void*)VIDMEM;
            void* terminalpointer = (void*)((VIDIDX + terminalCheck) * KBPAGE);
            memcpy(terminalpointer, vidpointer, COPYSIZE);
            if (terminalCheck == 1) {
                screen_x1 = screen_x;
                screen_y1 = screen_y;
                newlinecheck1 = newlinecheck;
            }

            if (terminalCheck == 3) {
                screen_x3 = screen_x;
                screen_y3 = screen_y;
                newlinecheck3 = newlinecheck;
            }
            //globalbuffer = globalbuffer2;
            count = &count2;
            terminalCheck = 2;
            screen_x = screen_x2;
            screen_y = screen_y2;
            newlinecheck = newlinecheck2;
            void* newterminalpointer = (void*)((VIDIDX + terminalCheck) * KBPAGE);
            memcpy(vidpointer, newterminalpointer, COPYSIZE);
            movingcursor(COL_COUNT * screen_y + screen_x);
            return;
        }
        //Terminal 3
        if (altpressed == 1 && input_scan_code == F3) {
            if (terminalCheck == 3) {
                return;
            }
            void* vidpointer = (void*)VIDMEM;
            void* terminalpointer = (void*)((VIDIDX + terminalCheck) * KBPAGE);
            memcpy(terminalpointer, vidpointer, COPYSIZE);
            if (terminalCheck == 1) {
                screen_x1 = screen_x;
                screen_y1 = screen_y;
                newlinecheck1 = newlinecheck;
            }

            if (terminalCheck == 2) {
                screen_x2 = screen_x;
                screen_y2 = screen_y;
                newlinecheck2 = newlinecheck;
            }
            //globalbuffer = globalbuffer3;
            count = &count3;
            terminalCheck = 3;
            screen_x = screen_x3;
            screen_y = screen_y3;
            newlinecheck = newlinecheck3;
            void* newterminalpointer = (void*)((VIDIDX + terminalCheck) * KBPAGE);
            memcpy(vidpointer, newterminalpointer, COPYSIZE);
            movingcursor(COL_COUNT * screen_y + screen_x);
            return;
        }
        //enter to goto next line
        if (input_scan_code == ENTERPRESSED) {
            switch (terminalCheck) {
            case 1:
                globalbuffer1[*count] = '\n';
                break;
            case 2:
                globalbuffer2[*count] = '\n';
                break;
            case 3:
                globalbuffer3[*count] = '\n';
                break;
            default:
                return;
            }
            //*globalbuffer = '\n';
            readflag = 1;
            newline_keyboard();
            *count = 0;
            return;
        }
        //backspace to delete last char shown
        if (input_scan_code == BACKSPACEPRESSED) {
            if (*count == 0) {
                return;
            }
            *count-=1;
            switch (terminalCheck) {
            case 1:
                globalbuffer1[*count] = 0;
                break;
            case 2:
                globalbuffer2[*count] = 0;
                break;
            case 3:
                globalbuffer3[*count] = 0;
                break;
            default:
                return;
            }
            //*globalbuffer--;
            //*globalbuffer = 0;
            backspace_keyboard();
            return;
        }
        //tab pressed to put a single space
        if (input_scan_code == TABPRESSED || input_scan_code == SPACEPRESSED) {
            if (maxflag != 1) {
                switch (terminalCheck) {
                case 1:
                    globalbuffer1[*count] = ' ';
                    break;
                case 2:
                    globalbuffer2[*count] = ' ';
                    break;
                case 3:
                    globalbuffer3[*count] = ' ';
                    break;
                default:
                    return;
                }
                //*globalbuffer = ' ';
                //*globalbuffer++;
                *count+=1;
                putc(' ');
            }
            return;
        }
        //capslocks swap (0 - off, 1 - on)
        if (input_scan_code == CAPSLOCK) {
            capslockactive = 1 - capslockactive;
        }
        //shift pressed and let go checks
        if (input_scan_code == LSHFTPRESSED || input_scan_code == RSHFTPRESSED) {
            shiftpressed = 1;
        }
        if (input_scan_code == LSHFTDEPRESSED || input_scan_code == RSHFTDEPRESSED) {
            shiftpressed = 0;
        }
        //crtl pressed and let go checks
        if (input_scan_code == CRTLPRESSED) {
            crtlpressed = 1;
        }
        if (input_scan_code == CRTLDEPRESSED) {
            crtlpressed = 0;
        }
        //alt pressed and let go checks
        if (input_scan_code == ALTPRESSED) {
            altpressed = 1;
        }
        if (input_scan_code == ALTDEPRESSED) {
            altpressed = 0;
        }
        //checking if the entered scan code matches the scan code at i
        int i;
        for (i = 0; i < NUM_CHARS; i++) {
        if (input_scan_code == scan_pressed_normal[i]) {
            if ((capslockactive == 0 && i < UPPERCASEOFFSET && shiftpressed == 0) || (capslockactive == 1 && shiftpressed == 1)) {
                if (maxflag != 1) {
                    switch (terminalCheck) {
                    case 1:
                        globalbuffer1[*count] = normal_chars[i];
                        break;
                    case 2:
                        globalbuffer2[*count] = normal_chars[i];
                        break;
                    case 3:
                        globalbuffer3[*count] = normal_chars[i];
                        break;
                    default:
                        return;
                    }
                    //*globalbuffer = normal_chars[i];
                    //*globalbuffer++;
                    *count+=1;
                    //prints lowercase
                    putc(normal_chars[i]);
                }
                break;
            }
            else {
                if (maxflag != 1) {
                    switch (terminalCheck) {
                    case 1:
                        globalbuffer1[*count] = normal_chars[i + UPPERCASEOFFSET];
                        break;
                    case 2:
                        globalbuffer2[*count] = normal_chars[i + UPPERCASEOFFSET];
                        break;
                    case 3:
                        globalbuffer3[*count] = normal_chars[i + UPPERCASEOFFSET];
                        break;
                    default:
                        return;
                    }
                    //*globalbuffer = normal_chars[i + UPPERCASEOFFSET];
                    //*globalbuffer++;
                    *count+=1;
                    //prints uppercase
                    putc(normal_chars[i + UPPERCASEOFFSET]);
                }
                break;
            }
        }
    }
}

/* open_terminal
 *
 * opens the keyboard for usage
 * Inputs: None
 * Outputs: None
 * Side Effects: none
 */
int32_t open_terminal(const uint8_t* filename) {
    return 0;
}

/* closes_terminal
 *
 * closes the keyboard for usage
 * Inputs: None
 * Outputs: None
 * Side Effects: none
 */
int32_t close_terminal(int32_t fd) {
    return 0;
}

/* read_terminal
 *
 * reads input from the keyboard buffer
 * Inputs: fd - file descriptor
 *              buf - buffer to write to
 *              nbytes - amount of data in bytes
 * Outputs: the amount of bytes we wrote to the buffer
 * Side Effects: fills buf without the chars we want to write
 */
int32_t read_terminal(int32_t fd, void* buf, int32_t nbytes) {
    int32_t copysize;
    //check if we want to read from terminal, if user hit enter
    while (readflag != 1) {}
    //error checking
    if (buf == NULL || nbytes < 0) {
        return -1;
    }
    if (terminalCheck == 1) {
        //check if we want to copy nbytes or full buffer size of data
        if (nbytes < sizeof(globalbuffer1)) {
            memcpy(buf, (void*)globalbuffer1, nbytes);
            copysize = nbytes;
        }
        else {
            memcpy(buf, (void*)globalbuffer1, sizeof(globalbuffer1));
            copysize = sizeof(globalbuffer1);
        }
        //reset buffer
        int i;
        for (i = 0; i < BUFFERLIMIT; i++) {
            globalbuffer1[i] = 0;
        }
    }
    if (terminalCheck == 2) {
        if (nbytes < sizeof(globalbuffer2)) {
            memcpy(buf, (void*)globalbuffer2, nbytes);
            copysize = nbytes;
        }
        else {
            memcpy(buf, (void*)globalbuffer2, sizeof(globalbuffer2));
            copysize = sizeof(globalbuffer2);
        }
        int i;
        for (i = 0; i < BUFFERLIMIT; i++) {
            globalbuffer2[i] = 0;
        }
    }
    if (terminalCheck == 3) {
        if (nbytes < sizeof(globalbuffer3)) {
            memcpy(buf, (void*)globalbuffer3, nbytes);
            copysize = nbytes;
        }
        else {
            memcpy(buf, (void*)globalbuffer3, sizeof(globalbuffer3));
            copysize = sizeof(globalbuffer3);
        }
        int i;
        for (i = 0; i < BUFFERLIMIT; i++) {
            globalbuffer3[i] = 0;
        }
    }
    //reset read from terminal flag
    readflag = 0;
    return copysize;
}

/* write_terminal
 *
 * writes chars from the buf to video memory
 * Inputs: fd - file descriptor
 *              buf - buffer we read input from
 *              nbytes - amount of data we want to write
 * Outputs: the amount of bytes we wrote to video memory
 * Side Effects: displays our char buf to video memory
 */
int32_t write_terminal(int32_t fd, const void* buf, int32_t nbytes) {
    //error checking
    if (buf == NULL || nbytes < 0) {
        return -1;
    }
    //looping through buffer printing nbytes char to screen
    char* bufptr = (char*)buf;
    int i;
    for (i = 0; i < nbytes; i++) {
        putc(bufptr[i]);
    }
    return nbytes;
}

/* get_cur_terminal
 *
 * Returns the terminal number we are currently at
 * Inputs: None
 * Outputs: The terminal number we are currently at
 * Side Effects: None
 */
extern int get_cur_terminal(){
  return terminalCheck;
}
