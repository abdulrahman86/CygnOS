#ifndef	__PIC_COMMON_HEADER
#define	__PIC_COMMON_HEADER

#define	PORT_PIC1_COMMAND		0x20
#define	PORT_PIC1_DATA			0x21
#define	PORT_PIC2_COMMAND		0xA0
#define	PORT_PIC2_DATA			0xA1

#define	ICW1_ICW4			0x1
#define	ICW1_SNGL			0x2
#define	ICW1_CALL_INTRVL_4	0x4
#define	ICW1_LEVEL_TRIG		0x8
#define	ICW1_INIT			0x10

#define	ICW4_X86			0x1
#define	ICW4_AEOI			0x2
#define	ICW4_BUF_MASTER		0x4
#define	ICW4_BUF_MODE		0x8

#define	PIC_EOI				0x20

#endif
