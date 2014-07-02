#ifndef	__SCREEN_VGA_HEADER
#define	__SCREEN_VGA_HEADER

#include <stdint.h>

#define	PORT_CURSOR_INDEX	0x03D4
#define	CURSOR_HIGH_INDEX	0x0E
#define	CURSOR_LOW_INDEX	0x0F

#define	PORT_CURSOR_DATA	0x03D5

//width of VGA display
extern const uint8_t width_vga;
//height of VGA display
extern const uint8_t height_vga;

//X co-ordinate of next character to be printed
extern uint8_t x_pos;
//Y co-ordinate of next character to be printed
extern uint8_t y_pos;

//Return a value that can be placed at a location of the video memory, representing a character and its attribute
extern uint16_t make_vga_entry(uint8_t, char);
//display a character on the VGA screen
extern void print_char_vga(char);
//display a string on the VGA screen
extern void print_string_vga(char *);
//display a string on the last line of the VGA screen
extern void print_last_line(char *);
//clear the VGA screen
extern void clear_screen_vga();
//update position of the screen cursor
extern void update_cursor();

#endif
