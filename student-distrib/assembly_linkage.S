#define ASM     1

.globl keyboard_linkage
.globl rtc_linkage
.globl system_linkage
.globl idt_00_linkage, idt_01_linkage, idt_02_linkage
.globl idt_03_linkage, idt_04_linkage, idt_05_linkage
.globl idt_06_linkage, idt_07_linkage, idt_08_linkage
.globl idt_09_linkage, idt_0A_linkage, idt_0B_linkage
.globl idt_0C_linkage, idt_0D_linkage, idt_0E_linkage
.globl idt_0F_linkage, idt_10_linkage, idt_11_linkage
.globl idt_12_linkage, idt_13_linkage, idt_14_linkage
.globl idt_15_linkage


/* keyboard_linkage
 *
 * Assembly linkage function for keyboard_handler
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls keyboard_handler
 */
keyboard_linkage:
    pushal
    pushfl
    call keyboard_handler
    popfl
    popal
    iret

/* rtc_linkage
 *
 * Assembly linkage function for rtc_handler
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls rtc_handler
 */
rtc_linkage:
    pushal
    pushfl
    call rtc_handler
    popfl
    popal
    iret

/* system_linkage
 *
 * Assembly linkage function for system_call_handler
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls system_call_handler
 */
system_linkage:
    cli
    cmpl $1, %eax # range is 1-6. Checking if <6
    jl error
    cmpl $8, %eax # Since only 6 system calls work right now
    jg error
    decl %eax # decrement eax because array indexing starts at 0
    pushw   %gs # push all registers
    pushw   %fs
    pushw   %es
    pushw   %ds
    pushl   %ebp
    pushl   %edi
    pushl   %esi
    pushl   %edx
    pushl   %ecx
    pushl   %ebx
    call    *syscall_jump_table(, %eax, 4) # call jump table, 4 is used to correctly index
    jmp pop_registers
error:
		movl $-1, %eax # return -1
    jmp exit
pop_registers:
	   popl    %ebx # pop all registers
    popl    %ecx
    popl    %edx
    popl    %esi
    popl    %edi
    popl    %ebp
    popw    %ds
    popw    %es
    popw    %fs
    popw    %gs
exit:
    iret

syscall_jump_table:
		.long halt, execute, read, write, open, close, getargs, vidmap # , set_handler, sigreturn


/* idt_00_linkage
 *
 * Assembly linkage function for idt_00 handler
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_00 handler
*/
idt_00_linkage:
    pushal
    pushfl
    call idt_00
    popfl
    popal
    iret

/* idt_01_linkage
 *
 * Assembly linkage function for idt_01
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_01 handler
 */
idt_01_linkage:
    pushal
    pushfl
    call idt_01
    popfl
    popal
    iret

/* idt_02_linkage
 *
 * Assembly linkage function for idt_02
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_02 handler
 */
idt_02_linkage:
    pushal
    pushfl
    call idt_02
    popfl
    popal
    iret

/* idt_03_linkage
 *
 * Assembly linkage function for idt_03
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_03 handler
 */
idt_03_linkage:
    pushal
    pushfl
    call idt_03
    popfl
    popal
    iret

/* idt_04_linkage
 *
 * Assembly linkage function for idt_04
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_04 handler
 */
idt_04_linkage:
    pushal
    pushfl
    call idt_04
    popfl
    popal
    iret

/* idt_05_linkage
 *
 * Assembly linkage function for idt_05
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_05 handler
 */
idt_05_linkage:
    pushal
    pushfl
    call idt_05
    popfl
    popal
    iret

/* idt_06_linkage
 *
 * Assembly linkage function for idt_06
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_06 handler
 */
idt_06_linkage:
    pushal
    pushfl
    call idt_06
    popfl
    popal
    iret

/* idt_07_linkage
 *
 * Assembly linkage function for idt_07
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_07 handler
 */
idt_07_linkage:
    pushal
    pushfl
    call idt_07
    popfl
    popal
    iret

/* idt_08_linkage
 *
 * Assembly linkage function for idt_08
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_08 handler
 */
idt_08_linkage:
    pushal
    pushfl
    call idt_08
    popfl
    popal
    iret

/* idt_09_linkage
 *
 * Assembly linkage function for idt_09
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_09 handler
 */
idt_09_linkage:
    pushal
    pushfl
    call idt_09
    popfl
    popal
    iret

/* idt_0A_linkage
 *
 * Assembly linkage function for idt_0A
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_0A handler
 */
idt_0A_linkage:
    pushal
    pushfl
    call idt_0A
    popfl
    popal
    iret

/* idt_0B_linkage
 *
 * Assembly linkage function for idt_0B
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_0B handler
 */
idt_0B_linkage:
    pushal
    pushfl
    call idt_0B
    popfl
    popal
    iret

/* idt_0C_linkage
 *
 * Assembly linkage function for idt_0C
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_0C handler
 */
idt_0C_linkage:
    pushal
    pushfl
    call idt_0C
    popfl
    popal
    iret

/* idt_0D_linkage
 *
 * Assembly linkage function for idt_0D
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_0D handler
 */
idt_0D_linkage:
    pushal
    pushfl
    call idt_0D
    popfl
    popal
    iret

/* idt_0E_linkage
 *
 * Assembly linkage function for idt_0E
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_0E handler
 */
idt_0E_linkage:
    pushal
    pushfl
    call idt_0E
    popfl
    popal
    iret

/* idt_0F_linkage
 *
 * Assembly linkage function for idt_0F
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_0F handler
 */
idt_0F_linkage:
    pushal
    pushfl
    call idt_0F
    popfl
    popal
    iret

/* idt_10_linkage
 *
 * Assembly linkage function for idt_10
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_10 handler
 */
idt_10_linkage:
    pushal
    pushfl
    call idt_10
    popfl
    popal
    iret

/* idt_11_linkage
 *
 * Assembly linkage function for idt_11
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_11 handler
 */
idt_11_linkage:
    pushal
    pushfl
    call idt_11
    popfl
    popal
    iret

/* idt_12_linkage
 *
 * Assembly linkage function for idt_12
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_12 handler
 */
idt_12_linkage:
    pushal
    pushfl
    call idt_12
    popfl
    popal
    iret

/* idt_13_linkage
 *
 * Assembly linkage function for idt_13
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_13 handler
 */
idt_13_linkage:
    pushal
    pushfl
    call idt_13
    popfl
    popal
    iret

/* idt_14_linkage
 *
 * Assembly linkage function for idt_14
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_14 handler
 */
idt_14_linkage:
    pushal
    pushfl
    call idt_14
    popfl
    popal
    iret

/* idt_15_linkage
 *
 * Assembly linkage function for idt_15
 * Inputs: None
 * Outputs: None
 * Side Effects: saves and restores flags and registers also calls idt_15 handler
 */
idt_15_linkage:
    pushal
    pushfl
    call idt_15
    popfl
    popal
    iret
