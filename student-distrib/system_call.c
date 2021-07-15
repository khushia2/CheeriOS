#include "system_call.h"
#include "system_call_helper.h"
#include "lib.h"
#include "i8259.h"
#include "file_system.h"
#include "paging.h"
#include "rtc.h"
#include "keyboard.h"
#include "x86_desc.h"
#include "paging_enable.h"

#define USER_ESP 0x083FFFFC // 128MB + 4MB - 4
#define USER_TOP 0x08000000
#define VIDMEMLOC 0x08800000
#define USERIDXVIDMEM 34
int shell_count = 1;
uint32_t process_num = 0;
int programs_1 = 0;
int programs_2 = 0;
int programs_3 = 0;
int term1_active_pid = -1;
int term2_active_pid = -1;
int term3_active_pid = -1;

/* system_call_handler
 *
 * Handler for system call
 * Inputs: None
 * Outputs: None
 * Side Effects: tells user that system call occured by printing to terminal
 */
void system_call_handler()
{
    printf("System call");
}

/*File operations tables*/
fjt std_function_table = {read_terminal, write_terminal, open_terminal, close_terminal};
fjt rtc_function_table = {rtc_read, rtc_write, rtc_open, rtc_close};
fjt file_function_table = {file_read, file_write, file_open, file_close};
fjt dir_function_table = {dir_read, dir_write, dir_open, dir_close};

/* ------------ helper function in syscall to set fd ---------------- */

/* setup_FD
 *
 * Sets up all the values of the file directory
 * Inputs: curr_proc_PCB: The process block for which the file directory has to be set up
 * Outputs: None
 * Side Effects: Sets the fields of the file directory
 */
void setup_FD(pcb_t* curr_proc_PCB){
  //setup for stdin
  curr_proc_PCB->fd_arr[0].fjt_ptr = &std_function_table;
  curr_proc_PCB->fd_arr[0].inode = 0;
  curr_proc_PCB->fd_arr[0].file_pos = 0;
  curr_proc_PCB->fd_arr[0].flags = 1;
  //setup for stdout
  curr_proc_PCB->fd_arr[1].fjt_ptr = &std_function_table;
  curr_proc_PCB->fd_arr[1].inode = 0;
  curr_proc_PCB->fd_arr[1].file_pos = 0;
  curr_proc_PCB->fd_arr[1].flags = 1;
}

/* execute
 *
 * Loads and executes a new program
 * Inputs: command: sequence of words which gives information on program to be executed
 * Outputs: -1 or 256 or a value between 0-255 depending on whether the call is executed
 * Side Effects: Gives the processor to the new program until it terminates
 */
int32_t execute(const uint8_t* command){
  //magic numbers to check if the file is an executable
  cli();
  int8_t exec_check[EXEC_SIZE] = {ENTRY_1, ENTRY_2, ENTRY_3, ENTRY_4};
  int8_t exit_check[EXIT_SIZE] = "exit";
  int8_t shell_check[SHELL_SIZE] = "shell";
  exception_flag = 0;

  //basic argument validity checks
  if(command == NULL)
    return -1;
  if(strlen((int8_t*) command) == 0)
    return -1;

  /* --- extracting file name from command --- */
  int8_t file_name[MAX_FILENAME];
  uint8_t args[ARGLENGTH];
  int i;
  int j = 0;
  /* Gets the command name from the 'command' argument */
  for(i = 0; i < MAX_FILENAME; i++){
    if(command[i] == ' ' || command[i] == '\n'){
    	file_name[i] = '\0';
      args[0] = '\0';
      break;
    }
    file_name[i] = command[i];
  }
  /*if exit is called, exit to previous program */
  if (strncmp(exit_check, file_name, EXIT_SIZE) == 0) {
    /* bookkeping if curr terminal is 1*/
    if(get_cur_terminal()==1){
      if(programs_1 == 1){ return -1;}
      else{
        curr_PCB = (pcb_t*)(EIGHT_MB_VAL  - EIGHT_KB_VAL * term1_active_pid);
        term1_active_pid = curr_PCB->parent_pid;
        process_num--;
        programs_1--;
        return 0;
      }
    }
    else if(get_cur_terminal()==2){
      /* bookkeping if curr terminal is 2*/
      if(programs_2 == 1){ return -1;}
      else{
        curr_PCB = (pcb_t*)(EIGHT_MB_VAL  - EIGHT_KB_VAL * term2_active_pid);
        term2_active_pid = curr_PCB->parent_pid;
        process_num--;
        programs_2--;
        return 0;
      }
    }
    else{
      /* bookkeping if curr terminal is 3*/
      if(programs_3 == 1){ return -1;}
      else{
        curr_PCB = (pcb_t*)(EIGHT_MB_VAL  - EIGHT_KB_VAL * term3_active_pid);
        term3_active_pid = curr_PCB->parent_pid;
        process_num--;
        programs_3--;
        return 0;
      }
    }
  }
  /*removes the whitespace after command name */
  while(command[i] == ' '){
    i++;
  }
  /*gets the argumnets from the 'command' argument */
  while (j < ARGLENGTH && command[i] != ' ' && command[i] != '\0' && command[i] != '\n'){
      args[j] = command[i];
      j++;
      i++;
  }
  args[j] = '\0';

  /* --- executable file validity check --- */
  dentry_t dentry;
  int8_t return_value = 0;
  //obtaining dentry relevant to the filename
  int check = read_dentry_by_name(file_name, &dentry);
    if(check == -1){
      return -1;
    }

  int8_t buf[EXEC_SIZE];
  read_data(dentry.inode_num, 0, (void*)buf, EXEC_SIZE);
  //checking if the top 4 bytes match the magic numbers in exec_check
  if(strncmp(buf, exec_check, EXEC_SIZE) != 0){
    return -1;
  }

  /* --- set up paging --- */
  if (strncmp(shell_check, file_name, SHELL_SIZE) == 0) {
    /* if command is shell, set up paging for it */
      if (process_num < 6) {
          process_num++;
          process_page(process_num-1);

      }
      else {
          printf("Max processes reached\n"); //not necessary but good to have
          return 0;
      }
  }
  else {
    /* if program is other than shell, set up paging*/
    if(process_num < 6){
      process_num++;
      process_page(process_num-1);
    }
    else{
      printf("Max processes reached\n"); //not necessary but good to have
      return 0;
      }
    }

  /* correctly updates the number of programs in each terminal */
  if(get_cur_terminal() == 1){
    programs_1++;
  }
  if(get_cur_terminal() == 2){
    programs_2++;
  }
  if(get_cur_terminal() == 3){
    programs_3++;
  }

  /* --- flush tlb --- */
  flush_tlb();

  /* --- load file into memory --- */
  uint8_t entry_point_arr[EXEC_SIZE];
  int32_t program_length = FOUR_MB_VAL - PROGRAM_PAGE_OFFSET;
  read_data(dentry.inode_num, 0, (uint8_t*) PROGRAM_IMG, program_length);
  read_data(dentry.inode_num, VAL_24, entry_point_arr, EXEC_SIZE);
  uint32_t entry_point;
  //extracting entry point from bytes 24 to 27 of the executable
  entry_point = ((int32_t)entry_point_arr[3]<<VAL_24) | ((int32_t)entry_point_arr[2]<<VAL_16) | ((int32_t)entry_point_arr[1]<<VAL_8) | ((int32_t)entry_point_arr[0]);

  /*--- set up PCB for current process ---*/
  curr_PCB = (pcb_t*)(EIGHT_MB_VAL  - EIGHT_KB_VAL * process_num);
  strcpy((int8_t *)curr_PCB->process_args, (int8_t *)args); //puts the arguments in the process_args field of pcb

/* bookkeping if base shell is being executed */
  if(get_cur_terminal() == 1 && programs_1 == 1){
    curr_PCB->parent_pid = -1; //base shell
    term1_active_pid = 0;
  }
  if(get_cur_terminal() == 2 && programs_2 == 1){
    curr_PCB->parent_pid = -1; //base shell
    term2_active_pid = 0;
  }
  if(get_cur_terminal() == 3 && programs_3 == 1){
    curr_PCB->parent_pid = -1; //base shell
    term3_active_pid = 0;
  }

  /* correctly updates pcb structures */
  curr_PCB->terminal_num = get_cur_terminal();
  if(curr_PCB->terminal_num == 1){
    curr_PCB->parent_pid = term1_active_pid; // set parent pid
    curr_PCB->pid = process_num - 1;   // set pid
    term1_active_pid = curr_PCB->pid;
  }
  if(curr_PCB->terminal_num == 2){
    curr_PCB->parent_pid = term2_active_pid; // set parent pid
    curr_PCB->pid = process_num - 1;   // set pid
    term2_active_pid = curr_PCB->pid;
  }
  if(curr_PCB->terminal_num == 3){
    curr_PCB->parent_pid = term3_active_pid; // set parent pid
    curr_PCB->pid = process_num - 1;   // set pid
    term3_active_pid = curr_PCB->pid;
  }

  /* ------ initialize the FDs that dont have open files to default values --------- */
  for(i = MIN_VALID_FD; i<MAX_FILE_NUM; i++){
    curr_PCB->fd_arr[i].fjt_ptr = NULL;
    curr_PCB->fd_arr[i].inode = 0;
    curr_PCB->fd_arr[i].file_pos = 0;
    curr_PCB->fd_arr[i].flags = 0;
  }

  /* --- set fd for stdin and stdout --- */
  setup_FD(curr_PCB);

  /* --- set parent kernel stack pointer and base pointer --- */
  asm volatile("			\n\
				movl %%ebp, %0 	\n\
				movl %%esp, %1 	\n\
			"
			:"=d"(curr_PCB->kbp_parent), "=b"(curr_PCB->ksp_parent));


  /* --- set up TSS stuff --- */
  tss.ss0 = KERNEL_DS;
  tss.esp0 = EIGHT_MB_VAL - (EIGHT_KB_VAL*curr_PCB->pid) - EXEC_SIZE;

  /* --- set up stack for iret call and then make iret call --- */
  prep_iret(entry_point);
  /* --- getting the right return value from halt --- */
  asm volatile("                        \n\
            get_ret_val:  	\n\
            movb	%%bl, %0  \n\
            "
            : "=a" (return_value));
  //checking if an exception has been raised
  if(exception_flag == 1)
  {
    exception_flag = 0;
    return (int32_t)EXCEPTION_CODE;
  }

  return (int32_t)return_value;
}

/* halt
 *
 * Terminates a process
 * Inputs: status: status of the process
 * Outputs: None
 * Side Effects: Restores parent data and parent paging
 */
int32_t halt(uint8_t status){
  cli();
  int i;
  /* close all the FDs for curr_pcb
   * If parent exists, undo paging,
   * set esp, ebp and esp0 to parent values
   */
   if(get_cur_terminal()==1){
     if(programs_1 == 1){
       programs_1--; //updates num of processes in terminal 1
       process_num--; //updates the total num of processes
       execute((uint8_t*)"shell");
       return 0;
     }
   }
   if(get_cur_terminal()==2){
     if(programs_2 == 1){
       programs_2--; //updates num of processes in terminal 1
       process_num--; //updates the total num of processes
       execute((uint8_t*)"shell");
       return 0;
     }
 }
  if(get_cur_terminal()==3){
    if(programs_3 == 1){
      programs_3--; //updates num of processes in terminal 1
      process_num--; //updates the total num of processes
      execute((uint8_t*)"shell");
      return 0;
   }
 }

  if(curr_PCB->parent_pid != -1){
    for(i = 0; i<MAX_FILE_NUM; i++){
    close(i);
    }
    if(get_cur_terminal()==1){
      programs_1--; //updates num of processes in terminal 1
      process_num--; //updates the total num of processes
      curr_PCB = (pcb_t*)(EIGHT_MB_VAL  - EIGHT_KB_VAL * term1_active_pid);
      term1_active_pid = curr_PCB->parent_pid; //updates the active pid for terminal 1
      process_page(term1_active_pid);
    }
    if(get_cur_terminal()==2){
      programs_2--; //updates num of processes in terminal 2
      process_num--; //updates the total num of processes
      curr_PCB = (pcb_t*)(EIGHT_MB_VAL  - EIGHT_KB_VAL * term2_active_pid);
      term2_active_pid = curr_PCB->parent_pid; //updates the active pid for terminal 2
      process_page(term2_active_pid);
    }
    if(get_cur_terminal()==3){
      programs_3--; //updates num of processes in terminal 3
      process_num--; //updates the total num of processes
      curr_PCB = (pcb_t*)(EIGHT_MB_VAL  - EIGHT_KB_VAL * term3_active_pid);
      term3_active_pid = curr_PCB->parent_pid; //updates the active pid for terminal 3
      process_page(term3_active_pid);
    }
    /* --- flush tlb --- */
    flush_tlb();
  }


  //restore parent stack pointers
  uint32_t parent_ksp = curr_PCB->ksp_parent;
  uint32_t parent_kbp = curr_PCB->kbp_parent;

  //curr_PCB = (pcb_t*)(EIGHT_MB_VAL  - EIGHT_KB_VAL * 1);
  /*
  if(get_cur_terminal() == 1){
    curr_PCB = (pcb_t*)(EIGHT_MB_VAL  - EIGHT_KB_VAL * (term1_active_pid + 1));
  }
  if(get_cur_terminal() == 2){
    curr_PCB = (pcb_t*)(EIGHT_MB_VAL  - EIGHT_KB_VAL * (term2_active_pid + 1));
  }
  if(get_cur_terminal() == 3){
    curr_PCB = (pcb_t*)(EIGHT_MB_VAL  - EIGHT_KB_VAL * (term3_active_pid + 1));
  }
  */

  tss.ss0 = KERNEL_DS;
  tss.esp0 = EIGHT_MB_VAL - EXEC_SIZE;

  // assembly helper function that sets esp to parent
  asm volatile("                        \n\
            movb	%2, %%bl  \n\
            movl	%1, %%esp  \n\
            movl	%0, %%ebp  \n\
            jmp get_ret_val \n\
            "
            :
            : "r" (parent_kbp), "r"(parent_ksp), "r"(status)
            : "%ebx");
  vidmap_page[0].present = DISABLE;
  return 0;
}

/* open
 *
 * Find the file in the file system and assigns a file descriptor
 * Inputs: filename: name of the file to open
 * Outputs: None
 * Side Effects: an unused file descriptor is assigned
 */
int32_t open(const uint8_t* filename) {

    //uncomment to check how exceptions are handled
    /*int a = 0;
    int b = 2;
    int c = b/a;*/

    if (strlen((int8_t*) filename) == 0) {
        return -1;
    }
    //validate passed in file
    dentry_t dentry;
    int check = read_dentry_by_name((int8_t*)filename, &dentry);
    if (check == -1) {
        return -1;
    }
    int i;
    uint32_t validLoc = -1;
    //check all file descriptor locations for open spot
    for (i = 0; i < FD_LOC; i++) {
        if (curr_PCB->fd_arr[i + MIN_VALID_FD].flags == 0) {
            validLoc = (uint32_t)(i + MIN_VALID_FD);
            break;
        }
    }

    //exit with error code if no valid locations found
    if (validLoc == -1) {
        return -1;
    }

    /* initializing the fjt_ptr to the right function table
     * file_type 0 -> rtc file
     * file_type 1 -> directory
     * file__type 2 -> normal file
     */
    if(dentry.file_type == 0)
      curr_PCB->fd_arr[validLoc].fjt_ptr = &rtc_function_table;
    else if(dentry.file_type == 1)
      curr_PCB->fd_arr[validLoc].fjt_ptr = &dir_function_table;
    else if(dentry.file_type == MIN_VALID_FD)
      curr_PCB->fd_arr[validLoc].fjt_ptr = &file_function_table;

    //only set inode if file_type is 2
    if (dentry.file_type != MIN_VALID_FD) {
        curr_PCB->fd_arr[validLoc].inode = 0;
    }
    else {
        curr_PCB->fd_arr[validLoc].inode = dentry.inode_num;
    }

    curr_PCB->fd_arr[validLoc].file_pos = 0; //start of the file
    curr_PCB->fd_arr[validLoc].flags = 1; //setting flag to 1 since fd location is currently in use
    return validLoc; //serves as fd for other system calls
}

/* close
 *
 * Closes the file decriptor
 * Inputs: fd: file descriptor
 * Outputs: None
 * Side Effects: Closes the file descriptor and sets all values to zero
 */
int32_t close(int32_t fd){
   // checking if fd id valid
  if(fd < MIN_VALID_FD || fd > MAX_VALID_FD){
    return -1;}
  // checking if it is already closed
  if(curr_PCB->fd_arr[fd].flags == 0){
    return -1;}
  // else set all values to zero
  curr_PCB->fd_arr[fd].fjt_ptr = NULL;
  curr_PCB->fd_arr[fd].inode = 0;
  curr_PCB->fd_arr[fd].file_pos = 0;
  curr_PCB->fd_arr[fd].flags = 0;
  return 0; //success
}

/* read
 *
 * Calls the corresponding read function
 * Inputs: fd: file descriptor
 * 				 buf: buffer to read from
 * 				 nbytes: number of bytes to read
 * Outputs: None
 * Side Effects: Uses the file descriptor to call the corresponding read function
 */
int32_t read(int32_t fd, void* buf, int32_t nbytes){
  sti();
  // check if fd is in valid range
  if(fd < 0 || fd > MAX_VALID_FD)
    return -1;
  // fd=1 is stdout
  if(fd == 1)
    return -1;
  // check if buf is valid
  if(buf == NULL)
    return -1;
  // check if it is already closed
  if (curr_PCB->fd_arr[fd].flags == 0)
    return -1;
  //printf("Reached %d %s \n", fd, buf);
  int32_t ret_val = curr_PCB->fd_arr[fd].fjt_ptr->read(fd, buf, nbytes);
  return ret_val;
}

/* write
 *
 * Calls the corresponding write function
 * Inputs: fd: file descriptor
 * 				 buf: buffer to write to
 * 				 nbytes: number of bytes to write
 * Outputs: None
 * Side Effects: Uses the file descriptor to call the corresponding write function
 */
int32_t write(int32_t fd, const void* buf, int32_t nbytes){
  // check if fd is in valid range
  if(fd < 0 || fd > MAX_VALID_FD)
    return -1;
  // fd=0 is stdin
  if(fd == 0)
    return -1;
  // check if buf is valid
  if(buf == NULL)
    return -1;
  // check if it is already closed
  if(curr_PCB->fd_arr[fd].flags == 0)
    return -1;
  return curr_PCB->fd_arr[fd].fjt_ptr->write(fd, buf, nbytes);
}

/* vidmap
 *
 * Maps video memory for use in user space
 * Inputs: screen_start: user pointer we want to set to video memory
 * Outputs: None
 * Side Effects: Maps text-mode video memory into user space at pre-set virtual address
 */
int32_t vidmap(uint8_t** screen_start) {
    //oob pointer checking
    if ((uint32_t)screen_start == NULL) {
        return -1;
    }
    if (USER_TOP > (uint32_t)screen_start || (uint32_t)screen_start > USER_ESP+EXEC_SIZE) {
        return -1;
    }

    //make new page table at 136 to give ample space (location 136MB)
    page_directory[USERIDXVIDMEM].base_address = ((unsigned int)vidmap_page) >> EMPTY_BITS;
    page_directory[USERIDXVIDMEM].present = ENABLE;
    page_directory[USERIDXVIDMEM].user_supervisor = ENABLE;


    //set entry in page table to vid mem
    vidmap_page[0].base_address = VIDEO_MEM_BASE_ADDR;
    vidmap_page[0].avail = DISABLE;
    vidmap_page[0].global_page = DISABLE;
    vidmap_page[0].page_size = DISABLE;
    vidmap_page[0].accessed = DISABLE;
    vidmap_page[0].reserved = DISABLE;
    vidmap_page[0].cache_disabled = DISABLE;
    vidmap_page[0].write_through = DISABLE;
    vidmap_page[0].user_supervisor = ENABLE;
    vidmap_page[0].read_write = ENABLE;
    vidmap_page[0].present = ENABLE;

    //set rest of table entry to not present
    int i;
    for (i = 0; i < TOTAL_ENTRIES; i++) {
      if(i ==  0)
        continue;
        vidmap_page[i].base_address = DISABLE;
        vidmap_page[i].avail = DISABLE;
        vidmap_page[i].global_page = DISABLE;
        vidmap_page[i].page_size = DISABLE;
        vidmap_page[i].accessed = DISABLE;
        vidmap_page[i].reserved = DISABLE;
        vidmap_page[i].cache_disabled = ENABLE;
        vidmap_page[i].write_through = DISABLE;
        vidmap_page[i].user_supervisor = DISABLE;
        vidmap_page[i].read_write = DISABLE;
        vidmap_page[i].present = DISABLE;
    }
    *screen_start = (uint8_t*)(VIDMEMLOC); //screen_start points to vid_map[0] which is at 136MB
     flush_tlb();
     return 0;
}

/* getargs
 *
 *Reads the programs command-line argumnets into a user-level buffer
 * Inputs: buf: buffer to write to
 * 	       nbytes: number of bytes to write
 * Outputs: None
 * Side Effects: buf is filled with the program's arguments
 */
int32_t getargs(uint8_t* buf, int32_t nbytes){
  if(buf == NULL){
      return -1; //check if buf is a valid pointer
  }
  if(curr_PCB->process_args[0] == '\0'){
      return -1; //check if no argumnets
  }
  strncpy((int8_t *)buf, (int8_t *)curr_PCB->process_args, nbytes); //puts the argumnets in the user-level buffer
  return 0;
}

/* set_handler
 *
 *
 * Inputs: signum: number of signal
 * 		   handler_address: address to the handler
 * Outputs: -1 since currently not used
 * Side Effects: None
 */
int32_t set_handler(int32_t signum, void* handler_address) {
    return -1;
}

/* sigreturn
 *
 *
 * Inputs: void
 * Outputs: -1 since currently not used
 * Side Effects: None
 */
int32_t sigreturn(void) {
    return -1;
}
