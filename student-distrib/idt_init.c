/* idt_init.c - initilizes our IDT with exceptions we are
 *              going to handle
 *
 */

#include "multiboot.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "debug.h"
#include "tests.h"
#include "keyboard.h"
#include "system_call.h"
#include "assembly_linkage.h"
#include "idt_init.h"
/* idt_default
 *
 * Default/null exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that unknown exception occured by printing to terminal
 */
void idt_default() {
    cli();
    printf("Unknown exeception");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
}

/* idt_00
 *
 * Division by zero exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "divide by zero" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_00()
{
    cli();
    printf("Division by zero");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);;
    sti();
    //while (1);
}

/* idt_01
 *
 * Debug exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "debug" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_01()
{
    cli();
    printf("Debug");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* idt_02
 *
 * Non_maskable Interrupt exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Non_maskable Interrupt" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_02()
{
    cli();
    printf("Non-maskable Interrupt");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* idt_03
 *
 * Breakpoint exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Breakpoint" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_03()
{
    cli();
    printf("Breakpoint");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* idt_04
 * Overflow exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Overflow" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_04()
{
    cli();
    printf("Overflow");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
   // while (1);
}

/* idt_05
 * Bounds exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Bounds" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_05()
{
    cli();
    printf("Bounds");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* idt_06
 * Invalid Opcode exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Invalid Opcode" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_06()
{
    cli();
    printf("Invalid Opcode");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* idt_07
 * Coprocessor Not Available exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Coprocessor Not Available" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_07()
{
    cli();
    printf("Coprocessor Not Available");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* idt_08
 * Double Fault exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Double fault" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_08()
{
    cli();
    printf("Double Fault");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* idt_09
 * Coprocessor Segment Overrun exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Coprocessor Segment Overrun" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_09()
{
    cli();
    printf("Coprocessor Segment Overrun");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* idt_0A
 * Invalid Task State Segment exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Invalid Task State Segment" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_0A()
{
    cli();
    printf("Invalid Task State Segment");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* idt_0B
 * Segment not present exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Segment not present" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_0B()
{
    cli();
    printf("Segment not present");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* idt_0C
 * Stack Fault exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Stack Fault" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_0C()
{
    cli();
    printf("Stack Fault");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* idt_0D
 * General protection fault exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "General protection fault" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_0D()
{
    cli();
    printf("General protection fault");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
   // while (1);
}

/* idt_0E
 * Page fault exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Page fault" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_0E()
{
    cli();
    printf("Page Fault\n");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    // while (1);
}

/* idt_0F
 * Reserved by intel
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that this is reserved by intel
   and then freezes the screen
 */
void idt_0F()
{
    cli();
    printf("Reserved");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
   // while (1);
}

/* idt_10
 * Math fault exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Math fault" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_10()
{
    cli();
    printf("Math Fault");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* idt_11
 * Alignment Check exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Alignment Check" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_11()
{
    cli();
    printf("Alignment Check");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* idt_12
 * Machine Check exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Machine Check" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_12()
{
    cli();
    printf("Machine Check");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* idt_13
 * SIMD Floating Point Exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "SIMD Floating Point" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_13()
{
    cli();
    printf("SIMD Floating Point Exception");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* idt_14
 * Virtualization Exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Virtualization" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_14()
{
    cli();
    printf("Virtualization Exception");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}


/* idt_15
 * Control Protection Exception handler
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that "Control Protection" exception occured by printing to terminal
   and then freezes the screen
 */
void idt_15()
{
    cli();
    printf("Control Protection Exception");
    exception_flag = 1;
    halt((uint8_t)EX_NUM);
    sti();
    //while (1);
}

/* init_idt_entry
 *
 * Initializes the IDT
 * Inputs: None
 * Outputs: None
 * Side Effects: Fills the IDT appropriately for Intel defined exceptions,
   system call, rtc interrupts and keyboard interrupts
 */
void init_idt_entry() {
    int i;
    // initialize all the entries to the default handler
    for (i = 0; i < NUM_VEC; i++) {
      //always set seg_selecter to Kernel code segment
        idt[i].seg_selector = KERNEL_CS;
        idt[i].reserved4 = 0x00;
        idt[i].reserved3 = 0x01;
        // set this field to 0 when IDT entry is an interrupt gate (PIC related IDT entries)
        if (i >= EXP_START && i <= EXP_END) {
            idt[i].reserved3 = 0x00;
        }
        idt[i].reserved2 = 0x01;
        idt[i].reserved1 = 0x01;
        idt[i].size = 0x01;
        idt[i].reserved0 = 0x00;
        idt[i].dpl = 0x00;
        if(i == 0x80)
          idt[i].dpl = 0x03;
        idt[i].present = 0x01;
        // set offset fields by calling set_id_entry
        SET_IDT_ENTRY(idt[i], idt_default);
    }
    /*
       set offset fields by calling set_id_entry for all the Intel defined exceptions
       as well keyboard handler, rtc handler, and system call
       Note - here the IDT leads to the assembly linkage function where all registers and flags
       are saved after which the actual handler is called
    */
    SET_IDT_ENTRY(idt[0x00], idt_00_linkage);
    SET_IDT_ENTRY(idt[0x01], idt_01_linkage);
    SET_IDT_ENTRY(idt[0x02], idt_02_linkage);
    SET_IDT_ENTRY(idt[0x03], idt_03_linkage);
    SET_IDT_ENTRY(idt[0x04], idt_04_linkage);
    SET_IDT_ENTRY(idt[0x05], idt_05_linkage);
    SET_IDT_ENTRY(idt[0x06], idt_06_linkage);
    SET_IDT_ENTRY(idt[0x07], idt_07_linkage);
    SET_IDT_ENTRY(idt[0x08], idt_08_linkage);
    SET_IDT_ENTRY(idt[0x09], idt_09_linkage);
    SET_IDT_ENTRY(idt[0x0A], idt_0A_linkage);
    SET_IDT_ENTRY(idt[0x0B], idt_0B_linkage);
    SET_IDT_ENTRY(idt[0x0C], idt_0C_linkage);
    SET_IDT_ENTRY(idt[0x0D], idt_0D_linkage);
    SET_IDT_ENTRY(idt[0x0E], idt_0E_linkage);
    //SET_IDT_ENTRY(idt[0x0F], idt_0F_linkage); //not caling because reserved by intel
    SET_IDT_ENTRY(idt[0x10], idt_10_linkage);
    SET_IDT_ENTRY(idt[0x11], idt_11_linkage);
    SET_IDT_ENTRY(idt[0x12], idt_12_linkage);
    SET_IDT_ENTRY(idt[0x13], idt_13_linkage);
    //SET_IDT_ENTRY(idt[0x14], idt_14_linkage); //not caling because reserved by intel
    //SET_IDT_ENTRY(idt[0x15], idt_15_linkage); //not caling because reserved by intel
    SET_IDT_ENTRY(idt[0x21], keyboard_linkage);
    SET_IDT_ENTRY(idt[0x28], rtc_linkage);
    SET_IDT_ENTRY(idt[0x80], system_linkage);
    // load the address of IDT table
    lidt(idt_desc_ptr);
}
