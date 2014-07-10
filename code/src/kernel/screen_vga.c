#include <screen_vga.h>
#include <stddef.h>
#include <string.h>
#include <io.h>

uint8_t x_pos = 0;
uint8_t y_pos = 0;

static uint16_t *video_memory = (uint16_t *)0xB8000;

uint16_t make_vga_entry(uint8_t _char_attrib, char __char_ascii)
{
	uint16_t vga_entry = 0;
	
	vga_entry = (vga_entry | _char_attrib) << 8;
	vga_entry |= __char_ascii;
	
	return vga_entry;
}

void print_char_vga(char __char_ascii)
{
	uint8_t char_attrib = 0x0F;
	
	if(__char_ascii != '\n')
		video_memory[y_pos * WIDTH_VGA + (x_pos++)] = make_vga_entry(char_attrib, __char_ascii);
	
	if(x_pos == WIDTH_VGA || __char_ascii == '\n')
	{
		x_pos=0;
		if(++y_pos == HEIGHT_VGA-1)
			y_pos = 0;
	}
	
	update_cursor();
}

void print_string_vga(char *__message)
{
	size_t msg_index;
	
	for(msg_index = 0;msg_index < strlen(__message);msg_index++)
		print_char_vga(__message[msg_index]);
}

void print_last_line(char *__message)
{
	uint8_t x_pos_last_line = 0, y_pos_last_line = HEIGHT_VGA - 1;
	uint8_t char_attrib = 0x0F;
	size_t last_line_char;
	
	for(last_line_char = 0;last_line_char < strlen(__message);last_line_char++)
		video_memory[y_pos_last_line * WIDTH_VGA + (x_pos_last_line++)] = make_vga_entry(char_attrib, __message[last_line_char]);
}

void clear_screen_vga()
{
	uint16_t vga_screen_char;
	uint8_t char_ascii_blank = 0x20;
	uint8_t char_attrib = 0x0F;	
	
	uint16_t vga_entry = 0;
	vga_entry = (vga_entry | char_attrib) << 8;
	vga_entry |= char_ascii_blank;

	for(vga_screen_char = 0; vga_screen_char < WIDTH_VGA * HEIGHT_VGA; vga_screen_char++)
	{			
			video_memory[vga_screen_char] = vga_entry;
	}
	
	x_pos = 0;
	y_pos = 0;
	
	update_cursor();
}

void update_cursor()
{
	uint16_t video_mem_index;
	
	video_mem_index = y_pos * WIDTH_VGA + x_pos;

	outb(PORT_CURSOR_INDEX, CURSOR_HIGH_INDEX);	
	outb(PORT_CURSOR_DATA, (video_mem_index >> 8) & 0xFF);

	outb(PORT_CURSOR_INDEX, CURSOR_LOW_INDEX);		
	outb(PORT_CURSOR_DATA, video_mem_index & 0xFF);
}
