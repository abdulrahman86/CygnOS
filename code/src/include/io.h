#ifndef	__IO_HEADER
#define	__IO_HEADER

#include <stdint.h>

#define	IO_WAIT_COUNTER		1000

//read one byte from port number "__port"
static __inline uint8_t inb(uint16_t __port)
{
	uint8_t portval;	
	
	__asm__ __volatile__ ("inb %w1, %0":"=a"(portval):"Nd"(__port));
	
	return portval;
}

//send one byte "__byte" to port with port number "__port"
static __inline void outb(uint16_t __port, uint8_t __byte)
{
	__asm__ __volatile__ ("outb %b1, %w0"::"Nd"(__port), "a"(__byte));
}

#endif
