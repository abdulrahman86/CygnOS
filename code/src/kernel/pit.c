#include <pit.h>
#include <io.h>
#include <string.h>
#include <screen_vga.h>

#define  TIMER_SECONDS_LENGTH	10

//cur_intr_millis=number of milliseconds since last IRQ0, uptime_seconds=total number of seconds since startup
static uint8_t cur_intr_millis = 0, uptime_seconds = 0;

//get the count to be loaded into the timer using the specified frequency
static uint16_t get_count_from_frequency(uint16_t __frequency)
{
	uint16_t count = PIT_BASE_HZ/__frequency;
	
	return count;
}

void pit_callback(i686_regs __regs)
{
	cur_intr_millis++;
	
	if(cur_intr_millis == 100)
	{
		tick++;
	
		uptime_seconds++;
		
		cur_intr_millis = 0;
	}
}

void pit_write(uint8_t __pit_channel, uint16_t __frequency)
{
	uint8_t pit_out_command = (__pit_channel << 6) | 0x34;
	uint8_t pit_out_port = 0x40 | (__pit_channel & 0x0F);
	uint16_t reload_count;
	
	outb(PORT_PIT_COMMAND, pit_out_command);
	
	reload_count = get_count_from_frequency(__frequency);
	
	outb(pit_out_port, reload_count & 0xFF);
	outb(pit_out_port, (reload_count >> 8) & 0xFF);
}

