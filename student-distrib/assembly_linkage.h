#ifndef _ASSEMBLY_LINKAGE_H
#define _ASSEMBLY_LINKAGE_H
#include "types.h"
#define MAX_RANGE 6

/* Assembly linkage function for keyboard handler */
extern void keyboard_linkage();
/* Assembly linkage function for rtc handler */
extern void rtc_linkage();
/* Assembly linkage function for system call handler */
extern void system_linkage();
/* Assembly linkage function for idt_00 */
extern void idt_00_linkage();
/* Assembly linkage function for idt_01 */
extern void idt_01_linkage();
/* Assembly linkage function for idt_02 */
extern void idt_02_linkage();
/* Assembly linkage function for idt_03 */
extern void idt_03_linkage();
/* Assembly linkage function for idt_04 */
extern void idt_04_linkage();
/* Assembly linkage function for idt_05 */
extern void idt_05_linkage();
/* Assembly linkage function for idt_06 */
extern void idt_06_linkage();
/* Assembly linkage function for idt_07 */
extern void idt_07_linkage();
/* Assembly linkage function for idt_08 */
extern void idt_08_linkage();
/* Assembly linkage function for idt_09 */
extern void idt_09_linkage();
/* Assembly linkage function for idt_0A */
extern void idt_0A_linkage();
/* Assembly linkage function for idt_0B */
extern void idt_0B_linkage();
/* Assembly linkage function for idt_0C */
extern void idt_0C_linkage();
/* Assembly linkage function for idt_0D */
extern void idt_0D_linkage();
/* Assembly linkage function for idt_0E */
extern void idt_0E_linkage();
/* Assembly linkage function for idt_0F */
extern void idt_0F_linkage();
/* Assembly linkage function for idt_10 */
extern void idt_10_linkage();
/* Assembly linkage function for idt_11 */
extern void idt_11_linkage();
/* Assembly linkage function for idt_12 */
extern void idt_12_linkage();
/* Assembly linkage function for idt_13 */
extern void idt_13_linkage();
/* Assembly linkage function for idt_14 */
extern void idt_14_linkage();
/* Assembly linkage function for idt_15 */
extern void idt_15_linkage();
#endif
