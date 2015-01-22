#include <pmm.h>
#include <screen_vga.h>

//represents the total number of pages available in physical memory
static uint64_t pages_total = 0;

//represents the total amount of physical memory in KB
static uint64_t memory_total = 0;

//holds the address of the previous page that was marked free, so that the address of the next page can be written to it when iterating over the pages
static uint64_t prev_page = 0;

uint64_t next_page = 0;

static int prev_page_initialized = 0;

extern uint32_t _START, _END;

void pmm_init(uint64_t __memory_total)
{
	memory_total = __memory_total;
	pages_total = memory_total/PAGE_SIZE;
}

void pmm_region_init(uint64_t __base, uint64_t __length)
{
	uint64_t page_index;

	if(__base%PAGE_SIZE != 0)
		__base = (__base/PAGE_SIZE)*PAGE_SIZE + PAGE_SIZE;
	
	if(__length%PAGE_SIZE != 0)
		__length = (__length/PAGE_SIZE)*PAGE_SIZE;	
		
	for(page_index = __base; page_index < (__base + __length); page_index += PAGE_SIZE)
	{
		if(page_index < *(uint64_t *)&_START || page_index >= *(uint64_t *)&_END)
		{
			if(prev_page_initialized)
				*(uint64_t *)prev_page = page_index;
				
			prev_page = page_index;
			if(!prev_page_initialized)
				prev_page_initialized = 1;
		}
	}
}
		

