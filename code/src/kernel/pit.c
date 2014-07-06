#include <pit.h>
#include <io.h>
#include <string.h>
#include <screen_vga.h>

#define  TIMER_SECONDS_LENGTH	10

//cur_intr_millis=number of milliseconds since last IRQ0, uptime_seconds=total number of seconds since startup
static uint8_t cur_intr_millis = 0, uptime_seconds = 0;
//array containing the number of seconds to be displayed on screen
static char timer_message_seconds[TIMER_SECONDS_LENGTH] = {' ',' ',' ',' ',' ',' ',' ',' ',' ','0'};

//update the timer_message_seconds array by adding one second
static void update_timer_msg_seconds()
{
	uint8_t last_digit = timer_message_seconds[TIMER_SECONDS_LENGTH-1] - 48, carry, sum, msg_char = 1;
	
	last_digit++;
	
	do
	{
		carry = last_digit/10;
		sum = last_digit%10;
		timer_message_seconds[TIMER_SECONDS_LENGTH-msg_char] = sum+48;
		
		msg_char++;
		last_digit = ((timer_message_seconds[TIMER_SECONDS_LENGTH-msg_char]==' '?'0':timer_message_seconds[TIMER_SECONDS_LENGTH-msg_char])-48) + carry;
	}while(carry && msg_char<=TIMER_SECONDS_LENGTH);
}

//get the count to be loaded into the timer using the specified frequency
static uint16_t get_count_from_frequency(uint16_t __frequency)
{
	uint16_t count = PIT_BASE_HZ/__frequency;
	
	return count;
}

void update_screen_time()
{
	char timer_message[width_vga + 1];
	memset(timer_message, '\0', width_vga + 1);

	char *timer_message_prefix = "Seconds since startup: ";
	
	uint8_t timer_message_prefix_len = (uint8_t)strlen(timer_message_prefix);
	uint8_t timer_message_space_len =  width_vga - (timer_message_prefix_len + TIMER_SECONDS_LENGTH), timer_message_space_len_half = timer_message_space_len/2;
	
	uint8_t timer_message_index;
	
	for(timer_message_index=0;timer_message_index < timer_message_space_len_half;timer_message_index++)
		timer_message[timer_message_index] = ' ';
		
	for(timer_message_index=timer_message_space_len_half;timer_message_index < timer_message_space_len_half+timer_message_prefix_len;timer_message_index++)
		timer_message[timer_message_index] = timer_message_prefix[timer_message_index - timer_message_space_len_half];
	
	update_timer_msg_seconds();
	for(timer_message_index=timer_message_space_len_half+timer_message_prefix_len;timer_message_index < timer_message_space_len_half+timer_message_prefix_len+TIMER_SECONDS_LENGTH;timer_message_index++)
		timer_message[timer_message_index] = timer_message_seconds[timer_message_index - (timer_message_space_len_half+timer_message_prefix_len)];
	
	for(timer_message_index=timer_message_space_len_half+timer_message_prefix_len+TIMER_SECONDS_LENGTH;timer_message_index < width_vga;timer_message_index++)
		timer_message[timer_message_index] = ' ';
	
	print_last_line(timer_message);
}

void pit_callback(i686_regs __regs)
{
	cur_intr_millis++;
	
	if(cur_intr_millis == 100)
	{
		uptime_seconds++;
		update_screen_time();
		
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

