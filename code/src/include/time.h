#ifndef	__TIME_HEADER
#define	__TIME_HEADER

#include <stdint.h>

typedef struct _timespec
{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint16_t year;
}timespec;

#endif
