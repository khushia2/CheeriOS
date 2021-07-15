#define ASM 1

.globl enable_paging

/* enable_paging
 *
 * Enables paging
 * Inputs: None
 * Outputs: None
 * Side Effects: changes values of cr0, cr3 and cr4
 */
enable_paging:
    pushl %ebp
    movl %esp, %ebp   # set up stack frame
    movl $page_directory, %eax
    movl %eax, %cr3   # set cr3 to the address of the page directory

    movl %cr4, %eax
    # OR 0x00000010 with cr4 because you want to set bit 4 to high for 4mb pages
    orl  $0x00000010, %eax
    movl %eax, %cr4

    movl %cr0, %eax
    # OR 0x80000000 with cr0 because you want to set 31st bit to high to enable paging
    orl  $0x80000000, %eax
    movl %eax, %cr0
    leave
    ret
