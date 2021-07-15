/* file_system.h - Defines for handling file_system
 */
#ifndef _FILE_SYSTEM_H
#define _FILE_SYSTEM_H

#include "types.h"

#define NUM_FILES_NAME 32
#define DENTRY_RESERVED 24
#define NUM_FILES 63
#define RESERVED_BOOT  52
#define NUM_DATA_BLOCKS   1023
#define BLOCK_SIZE 4096

/*Structures for file system*/
typedef struct
{
  int8_t file_name[NUM_FILES_NAME];
  uint32_t file_type;
  uint32_t inode_num;
  uint8_t reserved[DENTRY_RESERVED];
}dentry_t;

typedef struct
{
  uint32_t num_dentries;
  uint32_t num_inodes;
  uint32_t num_datablocks;
  uint8_t reserved[RESERVED_BOOT];
  dentry_t dir_ent[NUM_FILES];
}boot_block_t;

typedef struct
{
  uint32_t length;
  uint32_t block_num[NUM_DATA_BLOCKS];

}inode_t;

// void test_file_names();
/*Initializes all the temporary structs/variables being used for file system module*/
void file_sys_init(uint32_t start_addr);
// void test_file_details(int8_t* file_name);
// void test_directory_read();

/*Finds dentry based on file name and fills in file_name, file_type and inode*/
int32_t read_dentry_by_name(const int8_t* fname, dentry_t* dentry);

/*Read data from the specified inode number*/
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

/*Function code to open the file*/
int32_t file_open(const uint8_t* filename);

/* Function code to close the file */
int32_t file_close(int32_t fd);

/*Reads data from the file and stores it into buf passed */
int32_t file_read(int32_t fd, void* buf, int32_t nbytes);

/* Doesnt do anything because we are not allowed to write */
int32_t file_write (int32_t fd, const void* buf, int32_t nbytes);

/* Function code to open the directory */
int32_t dir_open(const uint8_t* filename);

/* Function code to close the directory */
int32_t dir_close(int32_t fd);

/* Reads and provides name of file pertaining to directory */
int32_t dir_read(int32_t fd, void* buf, int32_t nbytes);

/* Doesnt do anything because we are not allowed to write */
int32_t dir_write (int32_t fd, const void* buf, int32_t nbytes);

#endif
