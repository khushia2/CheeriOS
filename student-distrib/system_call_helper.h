#ifndef _SYSTEM_CALL_HELPER_H
#define _SYSTEM_CALL_HELPER_H

#include "types.h"
#include "system_call.h"
#include "x86_desc.h"

#define USER_DS    0x002B
#define USER_CS    0x0023
#define LABEL_NAME 0x083FFFFC
#define RV         0x002B

/* Flushes all TLB entries*/
extern void flush_tlb();

/* Assembly code for bookkeeping before returning to user code*/
extern void prep_iret(uint32_t entry_point);
//extern void halt_helper(uint32_t parent_kbp, uint32_t parent_ksp, uint8_t status);

#endif
