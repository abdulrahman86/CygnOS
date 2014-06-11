#include <stddef.h>
#include <stdint.h>
#include <gdt.h>

static const size_t width_vga  = 80;
static const size_t height_vga = 25;

static uint16_t *video_memory = (uint16_t *)0xB8000;

static size_t x_pos = 0;
static size_t y_pos = 0;

uint16_t make_vga_entry(uint8_t char_attrib, char char_ascii)
{
	uint16_t vga_entry = 0;
	
	vga_entry = (vga_entry | char_attrib) << 8;
	vga_entry |= char_ascii;
	
	return vga_entry;
}

size_t strlen(char *str)
{
	size_t length;
	
	for(length = 0;str[length] != '\0';length++);
	
	return length;
}

void print_char_vga(char char_ascii)
{
	uint8_t char_attrib = 0x0F;
	
	if(x_pos == width_vga || char_ascii == '\n')
	{
		x_pos=0;
		if(++y_pos == height_vga)
			y_pos = 0;
	}
	
	if(char_ascii != '\n')
	{
		video_memory[y_pos * width_vga + x_pos] = make_vga_entry(char_attrib, char_ascii);
		x_pos++;
	}
}

void print_string_vga(char *message)
{
	size_t msg_index;
	
	for(msg_index = 0;msg_index < strlen(message);msg_index++)
		print_char_vga(message[msg_index]);
}

void clear_screen_vga()
{
	uint16_t vga_screen_char;
	uint8_t char_ascii_blank = 0x20;
	uint8_t char_attrib = 0x0F;	
	
	uint16_t vga_entry = 0;
	vga_entry = (vga_entry | char_attrib) << 8;
	vga_entry |= char_ascii_blank;

	for(vga_screen_char = 0; vga_screen_char < width_vga * height_vga; vga_screen_char++)
	{			
			video_memory[vga_screen_char] = vga_entry;
	}
	
	x_pos = 0;
	y_pos = 0;
}

void main()
{
	char *starred_heading = "\n\n                    ****************************************                    ";
	char *welcome_message = "\n\n                              Welcome to Cygnus OS                              ";

	clear_screen_vga();
	
	print_string_vga(starred_heading);
	print_string_vga(welcome_message);
	print_string_vga(starred_heading);
}
