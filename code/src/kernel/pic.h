#include <io.h>

#define	PIC1_COMMAND		0x20
#define	PIC1_DATA			0x21
#define	PIC2_COMMAND		0xA0
#define	PIC2_DATA			0xA1

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
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	//slave PIC - x86 mode, ICW4 will be used
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);	
	
	//master PIC - use interrupt vectors 0x20 - 0x28
	outb(PIC1_DATA, 0x20);
	//slave PIC - use interrupt vectors 0x29 - 0x2F
	outb(PIC2_DATA, 0x28);
	
	//master PIC connected to slave PIC via IRQ 2 line
	outb(PIC1_DATA, 0x4);
	outb(PIC2_DATA, 0x2);
	
	//master ICW4 - x86 mode
	outb(PIC1_DATA, ICW4_X86);
	//slave ICW4 - x86 mode
	outb(PIC2_DATA, ICW4_X86);	
}

