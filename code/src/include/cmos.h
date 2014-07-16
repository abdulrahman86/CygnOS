#ifndef	__CMOS_HEADER
#define	__CMOS_HEADER

#include <io.h>

#define	PORT_CMOS_REG_SELECT	0x70
#define	PORT_CMOS_REG_RW		0x71

#define	REG_RTC_SECONDS			0x0
#define	REG_RTC_MINUTES			0x2
#define	REG_RTC_HOURS			0x4
#define	REG_RTC_WEEKDAY			0x6
#define	REG_RTC_MONTHDAY		0x7
#define	REG_RTC_MONTH			0x8
#define	REG_RTC_YEAR			0x9
#define	REG_RTC_STATUS_A		0xA
#define	REG_RTC_STATUS_B		0xB

//Not always
#define	REG_RTC_CENTURY		0x32

#define	CURRENT_YEAR		2014

//read value from a RTC register
uint8_t get_rtc_register(uint8_t __reg)
{
	outb(PORT_CMOS_REG_SELECT, __reg);
	return inb(PORT_CMOS_REG_RW);
}

//get status of RTC update in progress flag
int get_rtc_update_in_progress()
{
	outb(PORT_CMOS_REG_SELECT, REG_RTC_STATUS_A);
	return (inb(PORT_CMOS_REG_RW) & 0x80);
}

#endif
