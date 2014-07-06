#ifndef	__PIT_HEADER
#define	__PIT_HEADER

#include <idt.h>

#define	PIT_BASE_HZ				1193181
#define	PIT_FREQ_HZ				100

#define	PORT_PIT_COMMAND		0x43
#define	PORT_PIT_CHANNEL_0		0x40
#define	PORT_PIT_CHANNEL_1		0x41
#define	PORT_PIT_CHANNEL_2		0x42

//function called on IRQ0
extern void pit_callback(i686_regs);
//initialize the specified PIT channel and the load its timer count per the given frequency
extern void pit_write(uint8_t, uint16_t);
//update the time displayed on the screen
extern void update__screen_time();

#endif
