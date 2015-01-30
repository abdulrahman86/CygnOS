#include <pmm.h>
#include <string.h>
#include <screen_vga.h>

//represents the total number of pages available in physical memory
static uint64_t pages_total = 0;

//represents the total amount of physical memory in KB
static uint64_t memory_total = 0;

//holds the address of the previous page that was marked free, so that the address of the next page can be written to it when iterating over the pages
static uint32_t prev_page = 0;

uint32_t next_page = 0;

static int prev_page_initialized = 0;

extern uint32_t _START, _END;
static uint32_t kstart_addr, kend_addr;

void pmm_init(uint64_t __memory_total)
{
	memory_total = __memory_total;
	pages_total = memory_total/PAGE_SIZE;

	kstart_addr = (uint32_t)&_START;
	kend_addr = (uint32_t)&_END;
}

void pmm_region_init(uint32_t __base, uint32_t __length)
{
	uint32_t page_index;
	uint32_t region_end_addr = __base + __length;
	int kernel_in_region = 0;
	
	if(__base < kstart_addr)
	{
		if(region_end_addr > kstart_addr && region_end_addr <= kend_addr)
			__length -= (region_end_addr - __base);
		else if(region_end_addr > kend_addr)
			kernel_in_region = 1;
	}
	else if(__base < kend_addr)
	{
		if(region_end_addr <= kend_addr)
			return;
		
		__length -= (kend_addr - __base + 1);
		__base = kend_addr;
	}

	if(__base%PAGE_SIZE != 0)
		__base = (__base/PAGE_SIZE)*PAGE_SIZE + PAGE_SIZE;
	
	if(__length%PAGE_SIZE != 0)
		__length = (__length/PAGE_SIZE)*PAGE_SIZE;	
		
	for(page_index = __base; page_index < (__base + __length); page_index += PAGE_SIZE)
	{
		if(kernel_in_region && page_index >= kstart_addr && page_index < kend_addr)
			continue;
		
		*(uint32_t *)page_index = 0;
		if(prev_page_initialized)
			*(uint32_t *)prev_page = page_index;
				
		prev_page = page_index;
		if(!prev_page_initialized)
			prev_page_initialized = 1;
	}
}

uint32_t pmm_alloc()
{
	uint32_t next_page_buf;
	
	if(next_page)
	{
		next_page_buf = next_page;
		next_page = *(uint32_t *)next_page_buf;
		
		return next_page_buf;
	}
	else
		return 0;
}
		

