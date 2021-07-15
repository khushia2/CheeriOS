/* pcb.h - Defines used in the pcb
 * vim:ts=4 noexpandtab
 */

#ifndef _PCB_H
#define _PCB_H

#include "system_call.h"
#include "types.h"

#define MAX_FD 8
#define ARGLENGTH 128

/* Data structure for the file functions jump table */
typedef struct fjt
{
    int32_t (*read) (int32_t fd, void* buf, int32_t nbytes);
    int32_t (*write) (int32_t fd, const void* buf, int32_t nbytes);
    int32_t (*open) (const uint8_t* filename);
    int32_t (*close) (int32_t fd);
}fjt;

/* Data structure for the file descriptor */
typedef struct file_descriptor
{
    fjt* fjt_ptr;
    uint32_t inode;
    int32_t file_pos;
    int32_t flags;
}file_descriptor;

/* Data structure for the process control block*/
typedef struct pcb_t
{
    file_descriptor fd_arr[MAX_FD];
    uint32_t pid;
    uint32_t parent_pid;
    int terminal_num;
    uint32_t ksp_parent;
    uint32_t kbp_parent;
    uint8_t process_args[ARGLENGTH];
}pcb_t;

#endif /* _PCB_H */
