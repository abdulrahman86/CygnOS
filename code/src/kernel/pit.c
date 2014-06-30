#include <pit.h>
#include <io.h>

static uint16_t get_count_from_frequency(uint16_t __frequency)
{
	uint16_t count = PIT_BASE_HZ/__frequency;
	
	return count;
}

void pit_callback(i686_regs __regs)
{

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

