#include <stdint.h>

//read one byte from port number "__port"
static uint8_t __inline inb(uint8_t __port)
{
	uint8_t port_val;	
	
	__asm__ (".intel_syntax noprefix");
	__asm__ __volatile__ ("in %0, %1":"=a"(portval):"Nd"(__port));
}

//send one byte "__byte" to port with port number "__port"
static void __inline outb(uint8_t __port, uint8_t __byte)
{
	__asm__ ("intel_syntax noprefix");
	__asm__ __volatile__ ("out %0, %b1"::"Nd"(__port), "a"(__byte));
}
