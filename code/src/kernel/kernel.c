#include <io.h>
#include <gdt.h>
#include <idt.h>
#include <pit.h>
#include <pic.h>
#include <screen_vga.h>

void pic_remap()
{
	//master PIC - x86 mode, ICW4 will be used
	outb(PORT_PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	//slave PIC - x86 mode, ICW4 will be used
	outb(PORT_PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);	
	
	//master PIC - use interrupt vectors 0x20 - 0x27
	outb(PORT_PIC1_DATA, 0x20);
	//slave PIC - use interrupt vectors 0x28 - 0x2E
	outb(PORT_PIC2_DATA, 0x28);
	
	//master PIC connected to slave PIC via IRQ 2 line
	outb(PORT_PIC1_DATA, 0x4);
	outb(PORT_PIC2_DATA, 0x2);
	
	//master ICW4 - x86 mode
	outb(PORT_PIC1_DATA, ICW4_X86);
	//slave ICW4 - x86 mode
	outb(PORT_PIC2_DATA, ICW4_X86);	
	
	//enable only IRQ0
	outb(PORT_PIC1_DATA, 0xFE);
}

void kernel_infinite_loop()
{
	for(;;);
}

void main()
{
	char *starred_heading = "\n\n                    ****************************************                    ";
	char *welcome_message = "\n\n                              Welcome to Cygnus OS                              ";

	clear_screen_vga();
	
	print_string_vga(starred_heading);
	print_string_vga(welcome_message);
	print_string_vga(starred_heading);
	
	init_idt();
	setup_idt();

	pic_remap();
	
	init_time_msg_line();
	
	register_interrupt_handler(32, &pit_callback);
	pit_write(0, PIT_FREQ_HZ);	
		
	_i686_enable_interrupts();
	
	kernel_infinite_loop();
}
