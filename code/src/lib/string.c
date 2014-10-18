#include <string.h>

size_t strlen(char *__str)
{
	size_t length;
	
	for(length = 0;__str[length] != '\0';length++);
	
	return length;
}

char *strcat(char *__dest, char *__src)
{
	char *rdest = __dest;

	while(*__dest != '\0')
		__dest++;
		
	while(*__src != '\0')
		*__dest++ = *__src++;
	*__dest = '\0';
	
	return rdest;
}

void *memset(void *__dest, unsigned char __val, size_t __num_bytes)
{
	unsigned char *str = __dest;
	
	while(__num_bytes--)
		*(str++) = __val;	
		
	return __dest;
}

int insert_uint_in_str(char *__str, size_t __num_last_pos, uint64_t __num)
{
	if(__str == NULL || strlen(__str) <= __num_last_pos)
		return -1;
	
	while(__num > 0)
	{
		__str[__num_last_pos--] = __num%10 + 48;
		__num /= 10;
	}
	
	if(__num > 0)
		return -1;
	
	return 0;
}

