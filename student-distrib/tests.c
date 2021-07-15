#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "rtc.h"
#include "types.h"
#include "file_system.h"
#include "keyboard.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 *
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) &&
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

/* ---------------------------------------------------------------------- */
/* Testing Exception */

/* divide_by_zero
 *
 * Asserts that a divide by zero exception occurs when invoked
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: Invokes a divide by zero exception which freezes the screen
 */
void divide_by_zero(){
	printf("Test - Divide a number by zero ... \n");
	int a = 1;
	int b = 0;
	int temp;
	temp = a / b;
}

/* page_fault_exception
 *
 * Asserts that a page fault exception occurs when invoked
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: Invokes a page fault exception which freezes the screen
 */
void page_fault_exception(){
	printf("Test - Dereferencing NULL pointer ... \n");
	int* null_ptr = NULL;
	int temp;
	temp = *null_ptr;
}

/* OPTEST
 *
 * Asserts that an invalid opcode exception occurs when invoked
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: Invokes an invalid opcode exception which freezes the screen
 */
#define ERRORASM "movl %cr6, %eax"
#define OPTEST __asm__(ERRORASM)

/* ---------------------------------------------------------------------- */
/* Testing Paging */

/* dereference_nonex_page
 *
 * Asserts that a page fault exception occurs when invoked
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: Invokes a page fault exception which freezes the screen
 */
void dereference_nonex_page(){
	printf("Test - Dereferencing mem with non-existant page... \n");
	int* ptr = (int*)NON_EX_PAGE;
	int temp;
	temp = *ptr;
}

/* dereference_vid_mem
 *
 * Asserts that dereferencing video memory does NOT invoke page fault
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 */
void dereference_vid_mem(){
	// should not give an exception
	printf("Test - Dereferencing video memory... \n");
	int temp;
	int *ptr = (int*)VID_MEM;
	temp = *ptr;
	printf("Test passed. No exception raised.");
}

/* dereference_kernel
 *
 * Asserts that dereferencing kernel memory does NOT invoke page fault
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 */
void dereference_kernel(){
	printf("Test - Dereferencing kernel page... \n");
	int temp;
	int *ptr = (int*)KERNEL_MEM;
	temp = *ptr;
	printf("Test passed. No exception raised.");
}


/* dereference_nonex_mem
 *
 * Asserts that page fault occurs when memory with non-existant page is dereferenced
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: Invokes a page fault exception which freezes the screen
 */
void dereference_nonex_mem(){
	printf("Test - Dereferencing mem with non-existant page... \n");
	int temp;
	int *ptr = (int*)NON_EX_MEM;
	temp = *ptr;
}

/* Checkpoint 2 tests */
/* --------------------------------------------------------------------- */
/* Testing RTC Driver */


/* rtc_tests()
 *
 * Tests various functions of rtc such as open(), close(), read() and write() (changing the frequency)
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: prints appropriate statements about the function invoked and changes the frequency of rtc
 */
// void rtc_tests(){
// 	uint32_t buf[NUM_BYTES];
// 	int i;
// 	printf("Calling rtc_open, Frequency is 2 Hz. \n");
// 	rtc_open();
// 	for (i=0; i<LOOP_20; i++){
// 		rtc_read();
// 		printf("1");
// 	}
// 	printf("\nrtc_open call finished \n");
// 	printf("Calling rtc_read \n");
// 	rtc_read();
// 	printf("One interrupt occurred - rtc_read() call finished \n");
// 	printf("Changing frequency to 2 Hz\n");
// 	*buf = FREQ_2;
// 	rtc_write(buf, NUM_BYTES);
// 	/* provides enough time for enough interrupts to happen so that the change in frequency can be seen */
// 	for (i=0; i<LOOP_20; i++){
// 		rtc_read();
// 		printf("1");
// 	}
// 	printf("\nChanging frequency to 4 Hz\n");
// 	*buf = FREQ_4;
// 	rtc_write(buf, NUM_BYTES);
// 	/* provides enough time for enough interrupts to happen so that the change in frequency can be seen */
// 	for (i=0; i<LOOP_20; i++){
// 		rtc_read();
// 		printf("1");
// 	}
// 	printf("\nTrying invalid frequency \n");
// 	*buf = FREQ_GARB;
// 	rtc_write(buf, NUM_BYTES);
// 	for (i=0; i<LOOP_20; i++){
// 		rtc_read();
// 		printf("1");
// 	}
// 	printf("\nNo change because garbage value used.");
// 	printf("\nChanging frequency to 8 Hz\n");
// 	*buf = FREQ_8;
// 	rtc_write(buf, NUM_BYTES);
// 	/* provides enough time for enough interrupts to happen so that the change in frequency can be seen */
// 	for (i=0; i<LOOP_50; i++){
// 		rtc_read();
// 		printf("1");
// 	}
// 	printf("\nChanging frequency to 16 Hz\n");
// 	*buf = FREQ_16;
// 	rtc_write(buf, NUM_BYTES);
// 	/* provides enough time for enough interrupts to happen so that the change in frequency can be seen */
// 	for (i=0; i<LOOP_50; i++){
// 		rtc_read();
// 		printf("1");
// 	}
// 	printf("\nChanging frequency to 32 Hz\n");
// 	*buf = FREQ_32;
// 	rtc_write(buf, NUM_BYTES);
// 	/* provides enough time for enough interrupts to happen so that the change in frequency can be seen */
// 	for (i=0; i<LOOP_50; i++){
// 		rtc_read();
// 		printf("1");
// 	}
// 	printf("\nChanging frequency to 64 Hz\n");
// 	*buf = FREQ_64;
// 	rtc_write(buf, NUM_BYTES);
// 	/* provides enough time for enough interrupts to happen so that the change in frequency can be seen */
// 	for (i=0; i<LOOP_50; i++){
// 		rtc_read();
// 		printf("1");
// 	}
// 	printf("\nChanging frequency 128 Hz\n");
// 	*buf = FREQ_128;
// 	rtc_write(buf, NUM_BYTES);
// 	/* provides enough time for enough interrupts to happen so that the change in frequency can be seen */
// 	for (i=0; i<LOOP_100; i++){
// 		rtc_read();
// 		printf("1");
// 	}
// 	printf("\nChanging frequency to 256 Hz\n");
// 	*buf = FREQ_256;
// 	rtc_write(buf, NUM_BYTES);
// 	/* provides enough time for enough interrupts to happen so that the change in frequency can be seen */
// 	for (i=0; i<LOOP_100; i++){
// 		rtc_read();
// 		printf("1");
// 	}
// 	printf("\nChanging frequency to 512 Hz\n");
// 	*buf = FREQ_512;
// 	rtc_write(buf, NUM_BYTES);
// 	/* provides enough time for enough interrupts to happen so that the change in frequency can be seen */
// 	for (i=0; i<LOOP_100; i++){
// 		rtc_read();
// 		printf("1");
// 	}
// 	printf("\nChanging frequency to 1024 Hz\n");
// 	*buf = FREQ_1024;
// 	rtc_write(buf, NUM_BYTES);
// 	/* provides enough time for enough interrupts to happen so that the change in frequency can be seen */
// 	for (i=0; i<LOOP_100; i++){
// 		rtc_read();
// 		printf("1");
// 	}
// 	/* changing frequency back to 2 so that the screen is easier on the eyes and then testing rtc_close()*/
// 	*buf = FREQ_2;
// 	rtc_write(buf, NUM_BYTES);
// 	printf("\nCalling rtc_close()");
// 	rtc_close();
// 	printf("\nrtc_close() call finished \n");
// 	return;
// }
// /* ---------------------------------------------------------------------- */
// /* Testing File System Driver */

// /* test_file_sys_details
//  *
//  * Prints the contents of file pertaining to the file name passed as input
//  * Inputs: file_name = name of file
//  * Outputs: none
//  * Side Effects: Prints contents of file onto terminal
//  */
// void test_file_sys_details(int8_t* file_name){
//   test_file_details(file_name);
// }

// /* test_file_sys_names
//  *
//  * Function to test if all the file names are being read and
//  * displaying the filename, filesize and filetype
//  * Inputs: None
//  * Outputs: None
//  * Return value: None
//  * Side Effects: Prints the filename, filesize and filetype to
//  * 				of all the files to the terminal
//  */
//  void test_file_sys_names(){
//   test_file_names();
// }

// /* test_call_directory_read()
//  *
//  * Function to test if all the directory entries are being
//  * read and displaying the name of the directory entry
//  * Inputs: None
//  * Outputs: None
//  * Return value: None
//  * Side Effects: Prints the name of the directory entry
//  */
// void test_call_directory_read(){
// 	test_directory_read();
// }

// /* readwritekeyboardtest
//  *
//  * Function to test that our read and write terminal
//  *  functions correctly
//  * Inputs: None
//  * Outputs: None
//  * Return value: None
//  * Side Effects: puts our keyboard buffer into buffertest
// *                and display it to screen
//  */
 void readwritekeyboardtest() {
 	char buffertest[MAXBUFFER];
 	int32_t w, x, y, z;
 	//uncomment loop to test write with hardcoded amount
 	//comment while loop and read terminal line if you do
 	//int i;
 	//for (i = 0; i < ENTERBUFFERAMOUNTHERE; i++) {
 		//buffertest[i] = 'X';
 	//}
 	z = 0;
 	w = 100;


 	while (1) {
 		x = read_terminal(z, (void*)buffertest, w); //<- w decides how many char get added to our buffer
 		y = write_terminal(z, (void*)buffertest, x); // <- x decides how many char print to the screen
 	}
 }
// /* ---------------------------------------------------------------------- */

/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	/* Checkpoint 1 Test Cases */
	//TEST_OUTPUT("idt_test", idt_test());
	//divide_by_zero();
	//dereference_vid_mem();
    //dereference_kernel();
	//page_fault_exception();
	//dereference_nonex_mem();
	//dereference_nonex_page();
	//OPTEST;

	/* Checkpoint 2 Test Cases */
	//test_file_sys_names();
	//test_file_sys_details("grep");
	//test_file_sys_details("ls");
	//test_file_sys_details("frame0.txt");
	//test_file_sys_details("frame1.txt");
	//test_file_sys_details("verylargetextwithverylongname.txt");
	//test_file_sys_details("fish");
	//test_file_sys_details("gibberish");
	//test_call_directory_read();
	//rtc_tests();
	//readwritekeyboardtest();
}
