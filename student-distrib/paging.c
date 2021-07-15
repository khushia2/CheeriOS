/* paging.c - functions to initialize PTEs and PDEs
 *
 */
#include "lib.h"
#include "paging.h"
#include "paging_enable.h"


/* paging_init
 *
 * Initializes paging
 * Inputs: None
 * Outputs: None
 * Side Effects: Initializes the page table entries and the
 *               page directory entries before enabling paging.
 */
void paging_init(){
 unsigned int i;
 /* initializing page directory entries
  * with read/write enabled
  */
  for(i = 0; i < TOTAL_ENTRIES; i++){
    page_directory[i].base_address    = DISABLE;
    page_directory[i].avail           = DISABLE;
    page_directory[i].global_page     = DISABLE;
    page_directory[i].page_size       = DISABLE;
   	page_directory[i].accessed        = DISABLE;
    page_directory[i].reserved        = DISABLE;
   	page_directory[i].cache_disabled  = ENABLE;
   	page_directory[i].write_through   = DISABLE;
    page_directory[i].user_supervisor = DISABLE;
    page_directory[i].read_write      = ENABLE;
    page_directory[i].present         = DISABLE;
  }

  /* initializing the entry corresponding to the first 4MB */
  page_directory[0].base_address    = ((unsigned int)first_page_table) >> EMPTY_BITS;
  page_directory[0].present         = ENABLE; //setting the present bit to 1
  page_directory[0].read_write      = DISABLE;

  /* initializing the entry corresponding to the next 4MB which is the kernel */
  page_directory[1].base_address    = KERNEL_BASE_ADDR;
  page_directory[1].present         = ENABLE; //setting the present bit to 1
  page_directory[1].page_size       = ENABLE; //enabling the 4MB page

	/* initializing page table entries
  * with read/write enabled
  */
  for(i = 0; i < TOTAL_ENTRIES; i++)
	{
    /* initializing the page table entry for video memory */
    if(i == VIDEO_MEM_IDX)
    {
      first_page_table[i].base_address    = VIDEO_MEM_BASE_ADDR;
      first_page_table[i].avail           = DISABLE;
      first_page_table[i].global_page     = DISABLE;
      first_page_table[i].page_size       = DISABLE;
      first_page_table[i].accessed        = DISABLE;
      first_page_table[i].reserved        = DISABLE;
      first_page_table[i].cache_disabled  = DISABLE;
      first_page_table[i].write_through   = DISABLE;
      first_page_table[i].user_supervisor = DISABLE;
      first_page_table[i].read_write      = DISABLE;
      first_page_table[i].present         = ENABLE; //setting the present bit to 1
      continue;
    }
    /* initializing the pag table entries for terminal video memory*/
    if (i > VIDEO_MEM_IDX && i < VIDEO_MEM_IDX + MULTILIMIT) {
        first_page_table[i].base_address    = (i * PAGE_TABLE_ADDR) >> EMPTY_BITS;
        first_page_table[i].avail           = DISABLE;
        first_page_table[i].global_page     = DISABLE;
        first_page_table[i].page_size       = DISABLE;
        first_page_table[i].accessed        = DISABLE;
        first_page_table[i].reserved        = DISABLE;
        first_page_table[i].cache_disabled  = DISABLE;
        first_page_table[i].write_through   = DISABLE;
        first_page_table[i].user_supervisor = DISABLE;
        first_page_table[i].read_write      = DISABLE;
        first_page_table[i].present         = ENABLE; //setting the present bit to 1
        continue;
    }
    /* initializing the rest of the page table entries */
    first_page_table[i].base_address    = (i * PAGE_TABLE_ADDR) >> EMPTY_BITS;
    first_page_table[i].avail           = DISABLE;
    first_page_table[i].global_page     = DISABLE;
    first_page_table[i].page_size       = DISABLE;
    first_page_table[i].accessed        = DISABLE;
    first_page_table[i].reserved        = DISABLE;
    first_page_table[i].cache_disabled  = ENABLE;
    first_page_table[i].write_through   = DISABLE;
    first_page_table[i].user_supervisor = DISABLE;
    first_page_table[i].read_write      = DISABLE;
    first_page_table[i].present         = DISABLE;
	}
  enable_paging(); //enabling paging
}

/* process_page
 *
 * Enables entries of page directory
 * Inputs: idx: index into the page directory
 * Outputs: None
 * Side Effects: Sets the entries of the page directory
 */
void process_page(int idx) {
  page_directory[PROGRAM_PAGE_IDX].base_address    = (EIGHT_MB_VAL + idx * FOUR_MB_VAL)>>EMPTY_BITS;  //right shift so that only MSB are used
  page_directory[PROGRAM_PAGE_IDX].present         = ENABLE; //setting the present bit to 1
  page_directory[PROGRAM_PAGE_IDX].page_size       = ENABLE; //enabling the 4MB page
  page_directory[PROGRAM_PAGE_IDX].user_supervisor = ENABLE; // give user priviledge
}
