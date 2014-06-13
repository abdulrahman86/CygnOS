#ifndef	__SCREEN_VGA_HEADER
#define	__SCREEN_VGA_HEADER

#include <stdint.h>
#include <stddef.h>
#include <string.h>

//width of VGA display
extern const size_t width_vga;
//height of VGA display
extern const size_t height_vga;

//X co-ordinate of next character to be printed
extern size_t x_pos;
//Y co-ordinate of next character to be printed
extern size_t y_pos;

//Return a value that can be placed at a location of the video memory, representing a character and its attribute
uint16_t make_vga_entry(uint8_t, char);
//display a character on the VGA screen
void print_char_vga(char);
//display a string on the VGA screen
void print_string_vga(char *);
//clear the VGA screen
void clear_screen_vga();

#endif
