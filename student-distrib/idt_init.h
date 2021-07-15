/* idt_init.h - Defines used in our IDT table
 *
 * vim:ts=4 noexpandtab
 */

#ifndef _IDT_INIT_H
#define _IDT_INIT_H

#define EXP_START   0x20
#define EXP_END     0x2F
#define EX_NUM   256
#include "types.h"

/* initializes the Interrupt Descriptor table */
extern void init_idt_entry();

#endif /* _IDT_INIT_H */
