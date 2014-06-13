#include <string.h>

size_t strlen(char *__str)
{
	size_t length;
	
	for(length = 0;__str[length] != '\0';length++);
	
	return length;
}

void *memset(void *__dest, unsigned char __val, size_t __num_bytes)
{
	unsigned char *str = __dest;
	
	while(__num_bytes--)
		*(str++) = __val;	
		
	return __dest;
}
