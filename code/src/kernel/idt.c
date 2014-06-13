#include <idt.h>
#include <string.h>

//array representing and containing the entire IDT
static i686_idt_descriptor idt[IDT_MAX_DESCRIPTORS];

void setup_idt()
{
	idtr.idt_limit = sizeof(i686_idt_descriptor)*IDT_MAX_DESCRIPTORS - 1;
	idtr.idt_base  = (uint32_t)&idt;
	
	_i686_idt_install();
}

void set_i686_idt_descriptor(uint8_t __interrupt_index, I686_ISR_PTR __isr_base, uint16_t __interrupt_selector, uint8_t __interrupt_flags)
{
	idt[__interrupt_index].base_low  = (uint16_t)(((uint32_t)__isr_base) & 0xFFFF);
	idt[__interrupt_index].selector  = __interrupt_selector;
	idt[__interrupt_index].reserved  = 0;
	idt[__interrupt_index].flags     = __interrupt_flags;
	idt[__interrupt_index].base_high = (uint16_t)((((uint32_t)__isr_base) >> 16) & 0xFFFF);
}

void send_eoi_pic(uint8_t __irq)
{
	if(__irq >= 8)
		outb(PIC2_COMMAND, PIC_EOI);
	
	outb(PIC1_COMMAND, PIC_EOI);
}

void default_isr_handler()
{
	clear_screen_vga();
	print_string_vga("x86 unhandled interrupt: halting system.");
}

void init_idt()
{
	unsigned short int idt_index;
	
	memset((void *)&idt, 0, sizeof(i686_idt_descriptor)*IDT_MAX_DESCRIPTORS);

	for(idt_index = 0;idt_index < IDT_MAX_DESCRIPTORS;idt_index++)
		set_i686_idt_descriptor(idt_index, (I686_ISR_PTR)__ISR_DEFAULT, 0x8, I686_IDT_GATE_INT_DESCRIPTOR_32 | I686_IDT_SEGMENT_PRESENT);
}
