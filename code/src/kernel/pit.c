#include <pit.h>
#include <io.h>
#include <string.h>
#include <screen_vga.h>

#define  TIMER_SECONDS_LENGTH	10

//cur_intr_millis=number of milliseconds since last IRQ0, uptime_seconds=total number of seconds since startup
static uint8_t cur_intr_millis = 0, uptime_seconds = 0;
//array containing the number of seconds to be displayed on screen
static char time_message_seconds[TIMER_SECONDS_LENGTH] = {' ',' ',' ',' ',' ',' ',' ',' ',' ','0'};
//Prefix of the time message displayed in last line
static char *time_message_prefix = "Seconds since startup: ";
//array containing the entire actual message to de displayed on the last line
static char time_message[WIDTH_VGA + 1];
//lengths of the time message prefix and the length of the spaces within the last line
static uint8_t time_message_prefix_len, time_message_space_len;

//update the time_message_seconds array by adding one second
static void update_timer_msg_seconds()
{
	uint8_t last_digit = time_message_seconds[TIMER_SECONDS_LENGTH-1] - 48, carry, sum, msg_char = 1;
	
	last_digit++;
	
	do
	{
		carry = last_digit/10;
		sum = last_digit%10;
		time_message_seconds[TIMER_SECONDS_LENGTH-msg_char] = sum+48;
		
		msg_char++;
		last_digit = ((time_message_seconds[TIMER_SECONDS_LENGTH-msg_char]==' '?'0':time_message_seconds[TIMER_SECONDS_LENGTH-msg_char])-48) + carry;
	}while(carry && msg_char<=TIMER_SECONDS_LENGTH);
}

//get the count to be loaded into the timer using the specified frequency
static uint16_t get_count_from_frequency(uint16_t __frequency)
{
	uint16_t count = PIT_BASE_HZ/__frequency;
	
	return count;
}

void init_time_msg_line()
{
	uint8_t time_message_index;
	uint8_t time_message_space_len_half;

	memset(time_message, '\0', WIDTH_VGA + 1);
	
	time_message_prefix_len = (uint8_t)strlen(time_message_prefix);
	time_message_space_len =  WIDTH_VGA - (time_message_prefix_len + TIMER_SECONDS_LENGTH);
	time_message_space_len_half = time_message_space_len/2;
	
	for(time_message_index=0;time_message_index < time_message_space_len_half;time_message_index++)
		time_message[time_message_index] = ' ';
		
	for(time_message_index=time_message_space_len_half;time_message_index < time_message_space_len_half+time_message_prefix_len;time_message_index++)
		time_message[time_message_index] = time_message_prefix[time_message_index - time_message_space_len_half];
		
	for(time_message_index=time_message_space_len_half+time_message_prefix_len;time_message_index < WIDTH_VGA;time_message_index++)
		time_message[time_message_index] = ' ';
	
	print_last_line(time_message);
}

void update_screen_time()
{	
	uint8_t time_message_index;
	uint8_t time_message_space_len_half = time_message_space_len/2;
	
	update_timer_msg_seconds();
	for(time_message_index=time_message_space_len_half+time_message_prefix_len;time_message_index < time_message_space_len_half+time_message_prefix_len+TIMER_SECONDS_LENGTH;time_message_index++)
		time_message[time_message_index] = time_message_seconds[time_message_index - (time_message_space_len_half+time_message_prefix_len)];
	
	print_last_line(time_message);
}

void pit_callback(i686_regs __regs)
{
	cur_intr_millis++;
	
	if(cur_intr_millis == 100)
	{
		tick++;
	
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

