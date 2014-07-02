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

#endif
