#include <stdint.h>
#include <idt.h>

static i686_idt_descriptor idt[IDT_MAX_DESCRIPTORS];

uint8_t set_i686_idt_descriptor(uint8_t __interrupt_index, uint16_t __interrupt_selector, uint8_t __interrupt_flags)
{
	if(__interrupt_index < 0 || __interrupt_index > 255)
		return 0;
	
	idt[__interrupt_index].base_low  = ;
	idt[__interrupt_index].selector  = __interrupt_selector;
	idt[__interrupt_index].reserved  = 0;
	idt[__interrupt_index].flags     = __interrupt_flags;
	idt[__interrupt_index].base_high = ;
	
	return 1;
}
