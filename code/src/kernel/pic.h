#ifndef	__PIC_HEADER
#define	__PIC_HEADER

#include <io.h>
#include <stdint.h>
#include <pic_common.h>

#define	ICW1_ICW4			0x1
#define	ICW1_SNGL			0x2
#define	ICW1_CALL_INTRVL_4	0x4
#define	ICW1_LEVEL_TRIG		0x8
#define	ICW1_INIT			0x10

#define	ICW4_X86			0x1
#define	ICW4_AEOI			0x2
#define	ICW4_BUF_MASTER		0x4
#define	ICW4_BUF_MODE		0x8

//reconfigure the PIC to use interrupt vectors 32-47 (0x20-2F)
void pic_remap()
{
	//master PIC - x86 mode, ICW4 will be used
	outb(PORT_PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	//slave PIC - x86 mode, ICW4 will be used
	outb(PORT_PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);	
	
	//master PIC - use interrupt vectors 0x20 - 0x28
	outb(PORT_PIC1_DATA, 0x20);
	//slave PIC - use interrupt vectors 0x29 - 0x2F
	outb(PORT_PIC2_DATA, 0x28);
	
	//master PIC connected to slave PIC via IRQ 2 line
	outb(PORT_PIC1_DATA, 0x4);
	outb(PORT_PIC2_DATA, 0x2);
	
	//master ICW4 - x86 mode
	outb(PORT_PIC1_DATA, ICW4_X86);
	//slave ICW4 - x86 mode
	outb(PORT_PIC2_DATA, ICW4_X86);	
}

#endif
