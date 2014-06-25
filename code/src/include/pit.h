#ifndef	__PIT_HEADER
#define	__PIT_HEADER

#include <idt.h>

#define	PIT_BASE_HZ				1193181
#define	PIT_FREQ_HZ				100

#define	PORT_PIT_COMMAND		0x43
#define	PORT_PIT_CHANNEL_0		0x40
#define	PORT_PIT_CHANNEL_1		0x41
#define	PORT_PIT_CHANNEL_2		0x42

extern void pit_callback(i686_regs);

extern void pit_write(uint8_t, uint16_t);

#endif
