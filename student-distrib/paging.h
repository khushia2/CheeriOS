/* paging.h - Defines for handling paging initialization
 */
#ifndef _PAGING_H
#define _PAGING_H

#include "types.h"
#include "lib.h"
#include "x86_desc.h"

/* preventing magic numbers from wrecking us */
#define DISABLE 0x00
#define ENABLE 0x1
#define EMPTY_BITS 12
#define KERNEL_BASE_ADDR 0x400
#define VIDEO_MEM_BASE_ADDR 0xB8
#define VIDEO_MEM_IDX 184
#define TOTAL_ENTRIES 1024
#define PAGE_TABLE_ADDR 0x1000
#define EIGHT_MB_VAL 0x0800000
#define FOUR_MB_VAL 0x0400000
#define PROGRAM_PAGE_IDX 32
#define MULTILIMIT 4

/* function to initializing PTE and PDE */
extern void paging_init();
extern void process_page(int idx);

#endif
