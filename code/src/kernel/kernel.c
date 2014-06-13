#include <gdt.h>
#include <idt.h>
#include <io.h>
#include <screen_vga.h>
#include "pic.h"

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

	_i686_enable_interrupts();
//	pic_remap();
}
