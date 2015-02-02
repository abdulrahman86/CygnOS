#ifndef	__PMM_HEADER
#define	__PMM_HEADER

#include <stdint.h>
#include <multiboot.h>

#define	PAGE_SIZE	4096

//holds the address of the next free page in physical memory
extern uint32_t next_page;

//holds the address of the previous page that was marked free, so that the address of the next page can be written to it when iterating over the pages
extern uint32_t last_page;

//stores the total number of memory regions reported by the BIOS
extern uint16_t total_regions;

//this 32 entries long array will hold the memory map obtained from the multiboot struct
//unfilled enties will contain 0
extern multiboot_memory_map_t mmap[32];

//initialize the pmm by setting the total physical memory in KB and the total number of pages
extern void pmm_init(uint64_t __memory_total);

//adds the pages of the block of memory marked as available by the BIOS to the list of free pages.
//__base is the base address of the region, and __length is its length
extern void pmm_region_init(uint32_t __base, uint32_t __length);

//allocate a page for use by returning the next available free page from the list of free pages
//this is equivalent to returning the next_page which contains the above mentioned page's address.
//next_page is then updated to point to the next free page if it exists
//the allocated page is returned if it exists, else 0
extern uint32_t pmm_alloc();

//de-allocate a page i.e. return the specified page back to the list of free pages
//returns 0 if successful, else -1.
extern int pmm_dealloc(uint32_t);

#endif
