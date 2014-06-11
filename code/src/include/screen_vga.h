#ifndef	__SCREEN_VGA_HEADER
#define	__SCREEN_VGA_HEADER

#include <stdint.h>
#include <stddef.h>

extern const size_t width_vga;
extern const size_t height_vga;

uint16_t make_vga_entry(uint8_t, char);
size_t strlen(char *);
void print_char_vga(char);
void print_string_vga(char *);
void clear_screen_vga();

#endif
