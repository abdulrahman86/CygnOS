#include <pmm.h>

//represents the total number of pages available in physical memory
static uint64_t pages_total = 0;

//represents the total amount of physical memory in KB
static uint64_t memory_total = 0;

//holds the address of the next free page in physical memory
static uint64_t *next_page = 0xFFFFFFFFFFFFFFFF;

void pmm_init(uint64_t __memory_total)
{
	memory_total = __memory_total;
	pages_total = memory_total/PAGE_SIZE;
}

void pmm_region_init(uint64_t __base, uint64_t __length)
{
}
