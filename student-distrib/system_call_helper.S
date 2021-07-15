#define ASM 1

.globl flush_tlb
.globl prep_iret
.globl halt_helper

/* Flushes all TLB entries*/
flush_tlb:
		movl %cr3, %ebx
		movl %ebx, %cr3
    ret

/* Assembly code for bookkeeping before returning to user code*/
prep_iret:
    movl 4(%esp), %ebx # holds the value of EIP now
    xorl %eax, %eax
    movw $0x002B, %ax # USER_DS
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    pushl $0x002B # USER_DS check if the label works
    pushl $0x083FFFFC # 128MB + 4MB - 4
    # modify and push the flags
    pushfl
    # popl %edx
    # orl $0x0200, %edx
    # pushl %edx
    # flag modification and pushing done
    pushl $0x0023 # USER_CS check if the label works
    pushl %ebx
    iret
