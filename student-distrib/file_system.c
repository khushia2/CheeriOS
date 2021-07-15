/* file_system.c - functions to enable file system driver
 *
 *
 */
#include "file_system.h"
#include "lib.h"
#include "keyboard.h"
#include "system_call.h"

boot_block_t* boot_block;
inode_t* inode_start;
uint32_t data_block_start;
uint32_t dir_count = 0;

/* file_sys_init
 *
 * Initializes all the temporary structs/variables being used for file system module
 * Inputs: start_addr = starting address of file system module
 * Outputs: None
 * Side Effects: None
 */
void file_sys_init(uint32_t start_addr){
  /* copy contents from starting address of file system module
   * to the temporary variable containing boot block.
   * Also get the starting address of data blocks and inode blocks
  */
  //memcpy(&boot_block, (boot_block_t*)start_addr, BLOCK_SIZE);
  boot_block = (boot_block_t*)start_addr;
  inode_start = (inode_t*)(start_addr + BLOCK_SIZE);
  data_block_start = (uint32_t)inode_start + (boot_block->num_inodes * BLOCK_SIZE);
}

/* read_dentry_by_name
 *
 * Finds dentry based on file name and fills in file_name, file_type and inode #
 * Inputs: fname = name of file, dentry = directory entry that needs to be filled
 * Outputs: 0 = success, -1 = failure
 * Side Effects: None
 */
int32_t read_dentry_by_name(const int8_t* fname, dentry_t* dentry){
  int i;
  //check if dentry is invalid
  if(strlen(fname) > NUM_FILES_NAME){
    return -1;
  }
  if(dentry == NULL){
    return -1;
  }
  dentry_t* dentries = boot_block->dir_ent; //obtain dentries from boot block

  /* Loop through every dentry until you find one with the same file as the input,
   * Once found, fill file_name, file_type and inode #
   */
  for(i = 0; i<NUM_FILES; i++){
    if(!strncmp((dentries[i]).file_name, fname, NUM_FILES_NAME)){
      strcpy(dentry->file_name, (dentries[i]).file_name);
      dentry->file_type = (dentries[i]).file_type;
      dentry->inode_num = (dentries[i]).inode_num;
      return 0;
    }
  }
  return -1;
}

/* read_dentry_by_index
 *
 * Finds dentry based on dentry # and fills in file_name, file_type and inode #
 * Inputs: index = directory entry #, dentry = directory entry that needs to be filled
 * Outputs: 0 = success, -1 = failure
 * Side Effects: None
 */
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry){
  /* If dentry # (index) is out of bounds, return failure
   * else, fill dentry with file_name, file_type and inode #
   */

   //check if dentry is invalid
   if(dentry == NULL){
     return -1;
   }
  if(index < boot_block->num_dentries){
    dentry_t* dentries = boot_block->dir_ent;
    strcpy(dentry->file_name, (dentries[index]).file_name);
    dentry->file_type = (dentries[index]).file_type;
    dentry->inode_num = (dentries[index]).inode_num;
    return 0;
  }
  return -1;
}

/* read_data_block
 *
 * Reads the data from a certain position in the data block
 * till the end of the file or the block is reached and stores
 * the bytes in the buf
 * Inputs: block_num  = the current block number we are dealing with
 * 		   inode 	  = the current inode number we are dealing with
 * 		   start_byte = the starting byte for reading data from the block
 * 		   data_block_addr = the starting address of the given data block
 * 		   length	  = the total length of bytes that needs to be read
 * 		   bytes_read = the number of bytes read before this function call
 * Outputs: buf = stores the data read from the given data block
 * Return Values: curr_bytes_read = the number of bytes read in this block
 * Side Effects: None
 */
int32_t read_data_block(uint8_t block_num, uint32_t inode, uint8_t* buf, uint32_t start_byte,
                        uint32_t data_block_addr, uint32_t length, uint32_t bytes_read){
  //printf("Block Number: %d", block_num);
  uint32_t i; //loop counter
  uint32_t curr_bytes_read = 0; //number of bytes read in this box
  /* loop through each byte of the current data block */
  for(i = start_byte; i < BLOCK_SIZE; i++){
    /* check if the given length of bytes has been read */
    if(curr_bytes_read >= (length - bytes_read))
      break;
    /* check if the end of the file has been reached */
    if (((block_num) * BLOCK_SIZE + i) >= inode_start[inode].length)
    	break;
    uint8_t *temp = (uint8_t*)(data_block_addr + i);
   	buf[bytes_read + curr_bytes_read] = *temp; //copying the data stored at the given location in data block
    curr_bytes_read++; //incrementing the number of bytes read
  }
  //printf("Curr_Bytes: %d \n", curr_bytes_read);
  return curr_bytes_read;
}

/* read_data
 *
 * Read data from the specified inode number given an offset
 * and the number of bytes to be read. The read data is stored
 * in the passed buffer
 * Inputs: inode = the current inode number
 * 		   offset = the offset from which we need to start reading data
 * 		   length = the number of bytes to be read
 * Outputs: buf = holds the data that was read
 * Return value: bytes_read = the number of bytes read. Is equal to
 * 				 -1 if any of the input values are not valid
 * Side Effects: None
 */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
  // check if inode # is within bounds
  if(inode >= boot_block->num_inodes)
    return -1;
  //check if offset is within bounds
  if(inode_start[inode].length <= offset)
	  return -1;
  // get the total # of data blocks, data block # and the position within data block to start reading from
  uint32_t total_data_blocks = NUM_DATA_BLOCKS;//boot_block->num_datablocks;
  uint32_t curr_block_num = offset/BLOCK_SIZE;
  uint32_t curr_block_pos = offset%BLOCK_SIZE;
  uint32_t bytes_read = 0;
  //check if the block # is within bounds
  if(curr_block_num >= total_data_blocks){
    return -1;
  }
  // keep iterating until "length" number of bytes have been read or end of file has been reached
  while(1)
  {
    /* check if the specified length of bytes has been read */
    if(bytes_read >= length)
      break;
    /* check if end of file has been reached */
    if((offset + bytes_read) >= inode_start[inode].length)
      break;
    // get the starting address of data block corresponding to the data block you want to read from
    int32_t data_block_addr = data_block_start + BLOCK_SIZE*(inode_start[inode].block_num[curr_block_num]);
    /* update number of bytes read
     * get next data block, and next position within block
     */
    bytes_read += read_data_block(curr_block_num, inode, buf, curr_block_pos, data_block_addr, length, bytes_read);
    //printf("Buf is %s", buf);
    curr_block_num++;
    curr_block_pos = 0;
  }
  //printf("Bytes read %d", bytes_read);
  return bytes_read;
}

/* file_open
 *
 * Function code to open the file
 * Inputs: filename = name of the file that needs to be opened
 * Outputs: None
 * Return value: the value returned by read_dentry_by_name
 * 				 which is the success or failure code
 * Side Effects: None
 */
int32_t file_open(const uint8_t* filename){
  dentry_t* dentry; //declaring a dentry_t variable
  return read_dentry_by_name((int8_t*) filename, dentry);
}

/* file_close
 *
 * Function code to close the file
 * Inputs: None
 * Outputs: None
 * Return value: None
 * Side Effects: None
 */
int32_t file_close(int32_t fd){
  return 0;
}

/* file_read
 *
 * Reads data from the file and stores it into buf passed
 * Inputs: inode = inode number
 * 		   offset = position to start reading from
 * 		   buf = buffer into which we copy the data being read
 *         nbytes = number of bytes to read
 * Outputs: same as read_data which is the number of bytes read. Is equal to
 * 		    -1 if any of the input values are not valid
 * Side Effects: None
 */
int32_t file_read(int32_t fd, void* buf, int32_t nbytes){
  if(curr_PCB->fd_arr[fd].file_pos >= inode_start[curr_PCB->fd_arr[fd].inode].length)
    return 0;
  int ret_val;
   ret_val = read_data(curr_PCB->fd_arr[fd].inode, curr_PCB->fd_arr[fd].file_pos, buf, nbytes);    //read data using file system module function read_data()
  curr_PCB->fd_arr[fd].file_pos += ret_val;
  //printf("Retval is  %d /n", ret_val);
  return ret_val;
}

/* file_write
 *
 * Doesnt do anything because we are not allowed to write
 * Inputs (none being used here though): fd = file descriptor
 * 		      buf = buf that you need to write into the file
 * 		      nbytes = bytes to write
 * Outputs: -1 because you should not be able to write in our system
 * Side Effects: None
 */
int32_t file_write (int32_t fd, const void* buf, int32_t nbytes){
  return -1;
}

/* dir_open
 *
 * Function code to open the directory
 * Inputs: filename = name of the file that needs to be opened
 * Outputs: None
 * Return value: the value returned by read_dentry_by_name
 * 				 which is the success or failure code
 * Side Effects: None
 */
int32_t dir_open(const uint8_t* filename){
  dentry_t* dentry;
  return read_dentry_by_name((int8_t*)filename, dentry);
}

/* dir_close
 *
 * Function code to close the directory
 * Inputs: fd = file descriptor
 * Outputs: None
 * Return value: None
 * Side Effects: None
 */
int32_t dir_close(int32_t fd){
  return 0;
}

/* dir_read
 *
 * Reads and provides name of file pertaining to directory
 * Inputs: buf = buffer into which we copy the data being read
 *         nbytes = number of bytes to read
 * Outputs: 0 if the number of calls are greater than number of directories, else returns length of file_name
 * Side Effects: None
 */
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes){
  dentry_t dentry;
  //if number of calls made to dir_read exceeds number of dentries, return 0
  if(dir_count >= boot_block->num_dentries)
  {
    dir_count = 0; //to allow ls to work multiple times
    return 0;
  }
    //find dentry pertaining to index and get file_name
  int32_t result = read_dentry_by_index(dir_count, &dentry);
  //check if call was successful
  if(result == -1){
    return 0;
  }
  dir_count++;
  uint32_t i = 0;
  int32_t word_length = strlen(dentry.file_name);
  if(word_length >= NUM_FILES_NAME)
    word_length = NUM_FILES_NAME;
  for(i = 0; i < word_length; i++){
    ((int8_t*)buf)[i] = (dentry.file_name)[i];
  }
  return word_length; // return length of file_name
}

/* dir_write
 *
 * Doesnt do anything because we are not allowed to write
 * Inputs (none being used here though): fd = file descriptor
 * 		      buf = buf that you need to write into the file
 * 		      nbytes = bytes to write
 * Outputs: -1 because you should not be able to write in our system
 * Side Effects: None
 */
int32_t dir_write (int32_t fd, const void* buf, int32_t nbytes){
  return -1;
}

/* --------------Functions for Testing-------------- */

/* test_file_names
 *
 * Function to test if all the file names are being read and
 * displaying the filename, filesize and filetype
 * Inputs: None
 * Outputs: None
 * Return value: None
 * Side Effects: Prints the filename, filesize and filetype to
 * 				of all the files to the terminal
 */
// void test_file_names(){
//   clearscreen();//clear the screen
//   int i;
//   int j;
//   /* loop to go over all the files */
//   for(i = 0; i< boot_block->num_dentries; i++){
//     dentry_t dentry;
//     /* reading the file at the given index
//      * and receiving dentry as an output*/
//     int32_t result = read_dentry_by_index(i, &dentry);
//     if(result == -1){
//       printf("Invalid Arguments");
//       return;
//     }
//     printf("file_name: ");
//     int32_t file_name_len = strlen(dentry.file_name);
//     if(file_name_len >= NUM_FILES_NAME)
//       file_name_len = NUM_FILES_NAME;
//     //printing spaces to beautify
//     for(j = 0; j < (NUM_FILES_NAME - file_name_len); j++){
//       printf(" ");
//     }
//     write_terminal(0, dentry.file_name, file_name_len);
//     //printing the filetype and filesize
//     printf("   file_type:%d", dentry.file_type);
//     printf("   file_size:%d\n", inode_start[dentry.inode_num].length);
//   }
// }

/* test_file details
 *
 * Prints the contents of file pertaining to the file name passed as input
 * Inputs: file_name = name of file
 * Outputs: none
 * Side Effects: Prints contents of file onto terminal
 */
// void test_file_details(int8_t* file_name){
//   clearscreen();
//   dentry_t dentry;
//   //get the dentry pertaining to the file_name
//   int check = read_dentry_by_name(file_name, &dentry);
//   if(check == -1)
//   {
//     printf("File Does Not Exist");
//     return;
//   }
//   printf("File Name: ");
//   puts(dentry.file_name);
//   putc('\n');
//   //get length of file name
//   const int file_length = inode_start[dentry.inode_num].length;
//   int8_t buf[file_length];
//   //read from the file starting from the begining of the data block
//   file_read(dentry.inode_num, 0, buf, file_length);
//   write_terminal(0, buf, file_length);
//   putc('\n');
// }


/* test_directory_read
 *
 * Fuction that tests dir_read()
 * Inputs: none
 * Outputs: none
 * Side Effects: Prints the file names onto the terminal
 */
// void test_directory_read(){
//   clearscreen();
//   int i;
//   int j;
//   int8_t buf[NUM_FILES_NAME];
//   //loop to go over all the directory entries
//   for(i = 0; i < boot_block->num_dentries; i++){
//     //clears buffer
//     for(j = 0; j < NUM_FILES_NAME; j++){
//       buf[i] = ' ';
//     }
//     int len = dir_read(buf, NUM_FILES_NAME);
//     write_terminal(0, buf, len); //printing the current directory entry name
//     putc('\n');
//   }
// }
