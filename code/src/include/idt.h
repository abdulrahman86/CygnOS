#ifndef __IDT_HEADER
#define __IDT_HEADER

#include <stdint.h>

#define	IDT_MAX_DESCRIPTORS			256

#define I686_IDT_DESCRIPTOR_GATE_INT_16		0x06
#define	I686_IDT_DESCRIPTOR_GATE_INT_32		0x0E

#define	I686_IDT_GATE_TASK			0x05

#define	I686_IDT_GATE_TRAP_DESCRIPTOR_16	0x07
#define	I686_IDT_GATE_TRAP_DESCRIPTOR_32	0x17

#define	I686_IDT_DPL_1				0x20
#define	I686_IDT_DPL_2				0x40
#define	I686_IDT_DPL_3				0x60

#define	I686_IDT_SEGMENT_PRESENT		0x80

typedef struct __attribute__((packed))
{
	//low[0-15] 16 bits of ISR offset in the segment
	uint16_t	base_low;
	
	//segment/TSS selector pointing to segment containing the ISR
	uint16_t	selector;
	
	//reserved bits, keep 0
	uint8_t		reserved;
	
	uint8_t		flags;
	
	//high[16-31] 16 bits of ISR offset in the segment
	uint16_t	base_high;
} i686_idt_descriptor;

typedef struct __attribute__((packed))
{
	//size of the IDT - 1
	uint16_t	idt_limit;
	
	//base address of the IDT
	uint32_t	idt_base;
} i686_idtr_data;

typedef struct __attribute__((packed))
{
	//user space DS
	uint32_t ds;
	
	//all(non-segment) resgisters except EIP, ESP
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	
	//interrupt number and interrupt error code
	uint32_t interrupt_no, err_code;
	
	//registers pushed by the processor on interrupt
	uint32_t eip, cs, eflags, useresp, ss;
} i686_regs;

//typedef for pointer to a C interrupt handler
typedef void (*isr_ptr)(i686_regs);

//ISRs of each hardware interrupt
extern uint32_t IRQ0, IRQ1, IRQ2, IRQ3, IRQ4, IRQ5, IRQ6, IRQ7, IRQ8, IRQ9, IRQ10, IRQ11, IRQ12, IRQ13, IRQ14, IRQ15;

//struct representing contents to be loaded in IDTR
i686_idtr_data	idtr;

//install one descriptor in the IDT
extern void set_i686_idt_descriptor(uint8_t, uint32_t, uint16_t, uint8_t);
//register the C routine which will do the actual processing for the given interrupt
extern void register_interrupt_handler(uint8_t, isr_ptr);
//send the EOI command to the master/slave PIC(s)
extern void send_eoi_pic(uint8_t);
//fill in the i686_idtr_data struct and load IDTR
extern void setup_idt();

//load the IDTR
extern void _i686_idt_install();
//enable interrupts
extern void _i686_enable_interrupts();
//disable interrupts
extern void _i686_disable_interrupts();
//default ISR for un-implemented interrupts
extern void ISR_DEFAULT();

//C function for un-implemented interrupts or interrupts that have not registered any interrupt handler
extern void default_interrupt_handler();
//C function common to all ISRs called from isr.asm
//will call the appropriate interrupt handler based on the interrupt number
extern void common_interrupt_handler(i686_regs);
//initialize the IDT by inserting the same descriptor with default ISR in all entries
extern void init_idt();

#endif
