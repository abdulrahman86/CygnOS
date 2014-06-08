#ifndef __IDT_HEADER
#define __IDT_HEADER

#include <stdint.h>
#include <io.h>

#define	IDT_MAX_DESCRIPTORS			256

#define	I686_IDT_RESERVED			0x0

#define I686_IDT_GATE_INT_DESCRIPTOR_16		0x06
#define	I686_IDT_GATE_INT_DESCRIPTOR_32		0x0E

#define	I686_IDT_GATE_TASK			0x05

#define	I686_IDT_GATE_TRAP_DESCRIPTOR_16	0x07
#define	I686_IDT_GATE_TRAP_DESCRIPTOR_32	0x17

#define	I686_IDT_DPL_0				0x0
#define	I686_IDT_DPL_1				0x20
#define	I686_IDT_DPL_2				0x40
#define	I686_IDT_DPL_3				0x60

#define	I686_IDT_SEGMENT_PRESENT		0x80

typedef struct
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

i686_idtr_data	idtr;

extern void _i686_idt_install();

uint8_t set_i686_idt_descriptor(uint8_t, uint16_t, uint8_t);

#endif
