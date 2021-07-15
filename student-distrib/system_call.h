#ifndef _SYSTEM_CALL_H
#define _SYSTEM_CALL_H

#include "types.h"
#include "pcb.h"
#include "x86_desc.h"

#define MAX_FILENAME 32
#define MAX_FILE_NUM 8
#define FOUR_MB_VAL 0x0400000
#define PROGRAM_PAGE_OFFSET 0x048000
#define PROGRAM_IMG 0x08048000
#define EIGHT_KB_VAL 0x02000
#define EXEC_SIZE 4
#define SHELL_SIZE 5
#define ENTRY_1 0x7F
#define ENTRY_2 0x45
#define ENTRY_3 0x4C
#define ENTRY_4 0x46
#define FD_LOC 6
#define EXIT_SIZE 4
#define MIN_VALID_FD 2
#define MAX_VALID_FD 7
#define MAX_FILE_NUM 8
#define VAL_24 24
#define VAL_16 16
#define VAL_8  8
#define EXCEPTION_CODE 256
#define ARGLENGTH 128

int32_t exception_flag;
pcb_t * curr_PCB;

/* handler for system calls */
extern void system_call_handler();

/* Loads and executes a new program */
int32_t execute(const uint8_t* command);

/* Terminates a process */
int32_t halt(uint8_t status);

/* Finds the file in the file system and assigns a file descriptor */
int32_t open(const uint8_t* filename);

/* Closes the file decriptor */
int32_t close(int32_t fd);

/* Calls the corresponding read function */
int32_t read(int32_t fd, void* buf, int32_t nbytes);

/* Calls the corresponding write function */
int32_t write(int32_t fd, const void* buf, int32_t nbytes);

/* Gets pointer to video memory at user level */
int32_t vidmap(uint8_t** screen_start);

/* Parses args out of given buffer */
int32_t getargs(uint8_t* buf, int32_t nbytes);

/* Currently unused signal func */
int32_t set_handler(int32_t signum, void* handler_address);

/* Currently unused signal func */
int32_t sigreturn(void);

#endif
