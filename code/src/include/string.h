#ifndef	__STRING_HEADER
#define	__STRING_HEADER

#include <stdint.h>
#include <stddef.h>

//returns length of string(excluding '\0') str
extern size_t strlen(char *__str);

//set __num_bytes bytes to __val starting from __dest
//return __dest after filling in the memory.
extern void *memset(void *__dest, unsigned char __val, size_t __num_bytes);

#endif
