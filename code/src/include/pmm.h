#ifndef	__PMM_HEADER
#define	__PMM_HEADER

#include <stdint.h>

#define	PAGE_SIZE	4096

//initialize the pmm by setting the total physical memory in KB and the total number of pages
extern void pmm_init(uint64_t __memory_total);

//adds the pages of the block of memory marked as available by the BIOS to the list of free pages.
//__base is the base address of the region, and __length is its length
extern void pmm_init_region(uint64_t __base, uint64_t __length);

#endif
