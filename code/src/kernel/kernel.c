#include <io.h>
#include <gdt.h>
#include <idt.h>
#include <pit.h>
#include <pic.h>
#include <cmos.h>
#include <time.h>
#include <screen_vga.h>

static void pic_remap()
{
	//master PIC - x86 mode, ICW4 will be used
	outb(PORT_PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	//slave PIC - x86 mode, ICW4 will be used
	outb(PORT_PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);	
	
	//master PIC - use interrupt vectors 0x20 - 0x27
	outb(PORT_PIC1_DATA, 0x20);
	//slave PIC - use interrupt vectors 0x28 - 0x2E
	outb(PORT_PIC2_DATA, 0x28);
	
	//master PIC connected to slave PIC via IRQ 2 line
	outb(PORT_PIC1_DATA, 0x4);
	outb(PORT_PIC2_DATA, 0x2);
	
	//master ICW4 - x86 mode
	outb(PORT_PIC1_DATA, ICW4_X86);
	//slave ICW4 - x86 mode
	outb(PORT_PIC2_DATA, ICW4_X86);	
	
	//enable only IRQ0
	outb(PORT_PIC1_DATA, 0xFE);
}

static timespec get_rtc_time()
{
	uint8_t second, l_second;
	uint8_t minute, l_minute;
	uint8_t hour, l_hour;
	uint8_t day, l_day;
	uint8_t month, l_month;
	uint16_t year, l_year;
	uint8_t rtc_registerB;
	timespec rtc_time;
	
	while(get_rtc_update_in_progress());
	
	second = get_rtc_register(REG_RTC_SECONDS);
	minute = get_rtc_register(REG_RTC_MINUTES);
	hour = get_rtc_register(REG_RTC_HOURS);
	day = get_rtc_register(REG_RTC_MONTHDAY);
	month = get_rtc_register(REG_RTC_MONTH);
	year = get_rtc_register(REG_RTC_YEAR);
	
	do
	{
		l_second = second;
		l_minute = minute;
		l_hour = hour;
		l_day = day;
		l_month = month;
		l_year = year;
	
		while(get_rtc_update_in_progress());
	
		second = get_rtc_register(REG_RTC_SECONDS);
		minute = get_rtc_register(REG_RTC_MINUTES);
		hour = get_rtc_register(REG_RTC_HOURS);
		day = get_rtc_register(REG_RTC_MONTHDAY);
		month = get_rtc_register(REG_RTC_MONTH);
		year = get_rtc_register(REG_RTC_YEAR);
	}while((second!=l_second) || (minute!=l_minute) || (hour!=l_hour) || (day!=l_day) || (month!=l_month) || (year!=l_year));
	
	rtc_registerB = get_rtc_register(REG_RTC_STATUS_B);
	
	if(!(rtc_registerB & 0x4))
	{
		second = (second & 0xF) + ((second>>4)*10);
		minute = (minute & 0xF) + ((minute>>4)*10);		
		hour = (hour & 0xF) + ((((hour & 0x70)>>4)*10)|(hour & 0x80));
		day = (day & 0xF) + ((day>>4)*10);
		month = (month & 0xF) + ((month>>4)*10);
		year = (year & 0xF) + ((year>>4)*10);
	}
	
	if(!(rtc_registerB & 0x2) && (hour & 0x80))
		hour = ((hour & 0x7F) + 12)%24; 
	
	year += (CURRENT_YEAR/100) * 100;
	if(year < CURRENT_YEAR)
		year += 100;
		
	rtc_time.second = second;
	rtc_time.minute = minute;
	rtc_time.hour = hour;
	rtc_time.day = day;
	rtc_time.month = month;
	rtc_time.year = year;
	
	return rtc_time;
}

void kernel_infinite_loop()
{
	for(;;);
}

void main()
{
	char *starred_heading = "\n\n                    ****************************************                    ";
	char *welcome_message = "\n\n                              Welcome to Cygnus OS                              ";

	clear_screen_vga();
	
	print_string_vga(starred_heading);
	print_string_vga(welcome_message);
	print_string_vga(starred_heading);
	
	init_idt();
	setup_idt();

	pic_remap();
		
	register_interrupt_handler(32, &pit_callback);
	pit_write(0, PIT_FREQ_HZ);	

	rtc_init_time = get_rtc_time();	
	init_time_msg_line();
	tick = 0;
		
	_i686_enable_interrupts();
	
	kernel_infinite_loop();
}
