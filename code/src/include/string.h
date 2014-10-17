#ifndef	__STRING_HEADER
#define	__STRING_HEADER

#include <stdint.h>
#include <stddef.h>

//returns length of string(excluding '\0') str
extern size_t strlen(char *__str);

//concatenate __dest and __src into __dest, and return resulting __dest
//__dest must be large enough to hold both strings and the NULL character
extern char *strcat(char *__dest, char *__src);

//set __num_bytes bytes to __val starting from __dest
//return __dest after filling in the memory.
extern void *memset(void *__dest, unsigned char __val, size_t __num_bytes);

//inserts a number in a given string starting in reverse from the specified string index
//string length must be greater than the index given
//returns 0 on success, -1 in case of any error
extern int insert_uint_in_str(char *__str, size_t __num_last_pos, unsigned int __num);

#endif
