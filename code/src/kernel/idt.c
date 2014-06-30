#include <idt.h>
#include <string.h>
#include <io.h>
#include <pic.h>
#include <screen_vga.h>

//array representing and containing the entire IDT
static i686_idt_descriptor idt[IDT_MAX_DESCRIPTORS];

//array representing and containing the actual C routines for handling each interrupt
static isr_ptr interrupt_handlers[IDT_MAX_DESCRIPTORS];

void setup_idt()
{
	idtr.idt_limit = sizeof(i686_idt_descriptor)*IDT_MAX_DESCRIPTORS - 1;
	idtr.idt_base  = (uint32_t)&idt;
	
	_i686_idt_install();
}

void set_i686_idt_descriptor(uint8_t __interrupt_index, uint32_t __isr_base, uint16_t __interrupt_selector, uint8_t __interrupt_flags)
{
	idt[__interrupt_index].base_low  = (uint16_t)((__isr_base) & 0xFFFF);
	idt[__interrupt_index].selector  = __interrupt_selector;
	idt[__interrupt_index].reserved  = 0;
	idt[__interrupt_index].flags     = __interrupt_flags;
	idt[__interrupt_index].base_high = (uint16_t)(((__isr_base) >> 16) & 0xFFFF);
}

void register_interrupt_handler(uint8_t __interrupt_index, isr_ptr __interrupt_handler)
{	
	interrupt_handlers[__interrupt_index] = __interrupt_handler;
}

void send_eoi_pic(uint8_t __irq)
{
	if(__irq >= 8)
	{
		outb(PORT_PIC2_COMMAND, PIC_EOI);
	}
	
	outb(PORT_PIC1_COMMAND, PIC_EOI);
}

int check_spurious_irq(uint32_t __int_no)
{
	if(__int_no == 7)
	{
		outb(PORT_PIC1_COMMAND, 0x0B);
		uint8_t isr = inb(PORT_PIC1_COMMAND);
		if(!(isr & 0x80))
			return 1;
	}
	else if(__int_no == 15)
	{
		outb(PORT_PIC2_COMMAND, 0x0B);
		uint8_t isr = inb(PORT_PIC2_COMMAND);
		if(!(isr & 0x80))
		{
			outb(PORT_PIC1_COMMAND, PIC_EOI);
			return 1;
		}
	}
	
	return 0;
}

void common_interrupt_handler(i686_regs __regs)
{
	uint32_t int_no = __regs.interrupt_no;	
	
	if(!((int_no == 7 || int_no == 15) && check_spurious_irq(int_no)))
	{
		if(int_no >=32 && int_no <= 47)
			send_eoi_pic(int_no - 32);

		if(interrupt_handlers[int_no] != 0)
		{
			isr_ptr int_handler = interrupt_handlers[int_no];
			int_handler(__regs);
		}
	}
}

void default_interrupt_handler()
{
	clear_screen_vga();
	print_string_vga("x86 unhandled interrupt: halting system.");
}

void init_idt()
{
	unsigned short int idt_index;
	
	memset((void *)&idt, 0, sizeof(i686_idt_descriptor)*IDT_MAX_DESCRIPTORS);

	for(idt_index = 0;idt_index < IDT_MAX_DESCRIPTORS;idt_index++)
	{
		set_i686_idt_descriptor(idt_index, (uint32_t)&ISR_DEFAULT, 0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);
		register_interrupt_handler(idt_index, 0);
	}
	
	set_i686_idt_descriptor(32, (uint32_t)&IRQ0,  0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);
	set_i686_idt_descriptor(33, (uint32_t)&IRQ1,  0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);
	set_i686_idt_descriptor(34, (uint32_t)&IRQ2,  0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);
	set_i686_idt_descriptor(35, (uint32_t)&IRQ3,  0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);			
	set_i686_idt_descriptor(36, (uint32_t)&IRQ4,  0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);
	set_i686_idt_descriptor(37, (uint32_t)&IRQ5,  0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);
	set_i686_idt_descriptor(38, (uint32_t)&IRQ6,  0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);
	set_i686_idt_descriptor(39, (uint32_t)&IRQ7,  0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);			
	set_i686_idt_descriptor(40, (uint32_t)&IRQ8,  0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);
	set_i686_idt_descriptor(41, (uint32_t)&IRQ9,  0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);
	set_i686_idt_descriptor(42, (uint32_t)&IRQ10, 0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);
	set_i686_idt_descriptor(43, (uint32_t)&IRQ11, 0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);			
	set_i686_idt_descriptor(44, (uint32_t)&IRQ12, 0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);
	set_i686_idt_descriptor(45, (uint32_t)&IRQ13, 0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);
	set_i686_idt_descriptor(46, (uint32_t)&IRQ14, 0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);
	set_i686_idt_descriptor(47, (uint32_t)&IRQ15, 0x8, I686_IDT_DESCRIPTOR_GATE_INT_32 | I686_IDT_SEGMENT_PRESENT);
}
