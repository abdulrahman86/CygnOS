#include <io.h>
#include <gdt.h>
#include <idt.h>
#include <pit.h>
#include <pic.h>
#include <cmos.h>
#include <time.h>
#include <pmm.h>
#include <string.h>
#include <multiboot.h>
#include <screen_vga.h>
#include <mm/paging.h>

#define	ISLEAP(year)	(((year%4 == 0 && year%100 != 0) || (year%400 == 0))?1:0)
#define	CHECK_FLAGS_BIT(flags, bitnum)	(flags && (1<<bitnum))

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

static void store_mmap(uint32_t __mmap_addr, uint32_t __mmap_length)
{
	uint32_t addr_buf = __mmap_addr;
	multiboot_memory_map_t cur_map;
	int i = 0;
	
	while(addr_buf < (__mmap_addr + __mmap_length))
	{
		cur_map = *(multiboot_memory_map_t *)addr_buf;
		
		mmap[i].addr = cur_map.addr;
		mmap[i].len = cur_map.len;
		mmap[i].type = cur_map.type;
		mmap[i].size = cur_map.size;
		
		i++;
		
		addr_buf += cur_map.size + sizeof(cur_map.size);
	}
	
	total_regions = i;
}

static void init_mmap_and_memory(multiboot_info_t *mb_info)
{
	uint32_t mmap_addr = mb_info->mmap_addr;
	uint32_t mmap_length = mb_info->mmap_length;
	char *total_regions_usable_message = "\n                           Total region(s) usable                               ";
	
	memset((void *)mmap, 0, 32*sizeof(multiboot_memory_map_t));
	store_mmap(mmap_addr, mmap_length);
	
	multiboot_memory_map_t cur_map;	
	uint16_t regions_usable = 0;
	
	int next_page_initialized = 0, mmap_loop_index = 0;

	while(mmap_loop_index < total_regions)
	{
		cur_map = mmap[mmap_loop_index++];
		
		if(!next_page_initialized)
		{
			next_page = cur_map.addr%PAGE_SIZE?((cur_map.addr/PAGE_SIZE)*PAGE_SIZE + PAGE_SIZE):cur_map.addr;
			next_page_initialized = 1;
		}
		
		if(cur_map.type == MULTIBOOT_MEMORY_AVAILABLE)
		{
			pmm_region_init(cur_map.addr, cur_map.len);
			regions_usable++;
		}
	}
	
	if(!insert_uint_in_str(total_regions_usable_message, 53 /**52 + 1 for the newlines*/, regions_usable))
		print_string_vga(total_regions_usable_message);
}

static uint32_t get_init_seconds_since_epoch()
{
	uint32_t epoch_seconds = 0;
	uint32_t l_index;
	
	uint8_t month_days[2][12] = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
	
	for(l_index = 1970;l_index < rtc_init_time.year; l_index++)
		epoch_seconds += (ISLEAP(l_index)?366:365)*24*60*60;
	
	for(l_index = 1;l_index < rtc_init_time.month; l_index++)
		epoch_seconds += month_days[ISLEAP(rtc_init_time.year)][l_index-1]*24*60*60;
	
	epoch_seconds += (rtc_init_time.day-1)*24*60*60;
	epoch_seconds += rtc_init_time.hour*60*60;
	epoch_seconds += rtc_init_time.minute*60;
	epoch_seconds += rtc_init_time.second;
	
	return epoch_seconds;
}

void kernel_infinite_loop()
{
	for(;;);
}

void main(multiboot_info_t *mb_info)
{
	char *starred_heading = "\n\n                    ****************************************                    ";
	char *welcome_message = "\n\n                              Welcome to Cygnus OS                              ";
	char *memory_total_message = "\n\n                          Total system memory:      MB                          ";

	uint64_t memory_total = 0;

	clear_screen_vga();
	
	print_string_vga(starred_heading);
	print_string_vga(welcome_message);
	print_string_vga(starred_heading);
		
	rtc_init_time = get_rtc_time();
	tick = get_init_seconds_since_epoch();
	
	init_idt();
	setup_idt();

	pic_remap();
		
	register_interrupt_handler(32, &pit_callback);
	pit_write(0, PIT_FREQ_HZ);	

	if(CHECK_FLAGS_BIT(mb_info->flags, 0))
		memory_total = mb_info->mem_upper + 1024;
	pmm_init(memory_total);
		
	if(!insert_uint_in_str(memory_total_message, 53 /*51 + 2 newlines*/, memory_total/1024))
		print_string_vga(memory_total_message);
	
	if(CHECK_FLAGS_BIT(mb_info->flags, 6))
		init_mmap_and_memory(mb_info);
	
	_i686_enable_interrupts();	
	
	paging_startup();
	_i686_enable_paging();
	
	kernel_infinite_loop();
}
