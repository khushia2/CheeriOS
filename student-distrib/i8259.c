/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask = 0x00; /* IRQs 0-7  */
uint8_t slave_mask = 0x00;  /* IRQs 8-15 */


/* Initialize the 8259 PIC */
void i8259_init(void) {
  /* masking all the ports in master and slave PIC */
	outb(MASK_ALL, MASTER_8259_DATA_PORT);
	outb(MASK_ALL, SLAVE_8259_DATA_PORT);

	outb(ICW1, MASTER_8259_PORT);
	outb(ICW2_MASTER, MASTER_8259_DATA_PORT);
	outb(ICW3_MASTER, MASTER_8259_DATA_PORT);
  outb(ICW4, MASTER_8259_DATA_PORT);

	outb(ICW1, SLAVE_8259_PORT);
	outb(ICW2_SLAVE, SLAVE_8259_DATA_PORT);
	outb(ICW3_SLAVE, SLAVE_8259_DATA_PORT);
	outb(ICW4, SLAVE_8259_DATA_PORT);

  /* masking off all ports except the slave port on the master PIC */
	outb(MASK_MASTER, MASTER_8259_DATA_PORT);
  /* masking off all ports on the slave PIC */
	outb(MASK_ALL, SLAVE_8259_DATA_PORT);
}

/* Enable (unmask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
	unsigned char val;
  //dealing with IRQs on the master PIC
  if(irq_num < PICsize){
    val = inb(MASTER_8259_DATA_PORT) | (1<<irq_num);
    outb(val, MASTER_8259_DATA_PORT );
  }
  //dealing with IRQs on the slave PIC
  else{
    irq_num = irq_num - PICsize;
    val = inb(SLAVE_8259_DATA_PORT) | (1<<irq_num);
    outb(val, SLAVE_8259_DATA_PORT);
  }
}

/* Disable (mask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
	unsigned char val;
  //dealing with IRQs on the master PIC
  if(irq_num<PICsize){
    val = inb(MASTER_8259_DATA_PORT) & ~(1<<irq_num);
    outb(val, MASTER_8259_DATA_PORT);
  }
  //dealing with IRQs on the slave PIC
  else{
    irq_num = irq_num-PICsize;
    val = inb(SLAVE_8259_DATA_PORT) & ~(1<<irq_num);
    outb(val, SLAVE_8259_DATA_PORT);
  }
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
  unsigned char eoi_val = EOI;
	unsigned char master_eoi;
  //dealing with IRQs on the master PIC
  if(irq_num < PICsize){
    eoi_val = eoi_val | irq_num;
    outb(eoi_val, MASTER_8259_PORT);
  }
  //dealing with IRQs on the slave PIC
  else{
    eoi_val = eoi_val | (irq_num - PICsize);
		master_eoi = EOI|MASTER_EOI_MASK;
		outb(master_eoi, MASTER_8259_PORT);
    outb(eoi_val, SLAVE_8259_PORT);
  }
}
