#ifndef __IDT_HEADER
#define __IDT_HEADER

#include <io.h>
#include <stdint.h>
#include <pic_common.h>
#include <screen_vga.h>

#define	IDT_MAX_DESCRIPTORS			256

#define	I686_IDT_RESERVED			0x0

#define I686_IDT_GATE_INT_DESCRIPTOR_16		0x06
#define	I686_IDT_GATE_INT_DESCRIPTOR_32		0x0E

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

//typedef for pointer to an ISR
typedef void (*I686_ISR_PTR)();

//struct representing contents to be loaded in IDTR
i686_idtr_data	idtr;

//install one descriptor in the IDT
extern void set_i686_idt_descriptor(uint8_t, I686_ISR_PTR, uint16_t, uint8_t);
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
extern void __ISR_DEFAULT();

//C function for handling the default ISR, called from isr.asm
extern void default_isr_handler();
//initialize the IDT by inserting the same descriptor with default ISR in all entries
extern void init_idt();

#endif
