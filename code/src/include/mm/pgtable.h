#ifndef	__PGTABLE_HEADER
#define	__PGTABLE_HEADER

#include <stdint.h>

#define	PAGE_TABLE_ENTRIES		1024

//bit masks for the different bits of a 32-bit non-PAE page table entry
#define	PAGE_PRESENT			0x00000001
#define	PAGE_RW					0x00000002
#define	PAGE_PRIVILEGE			0x00000004
#define	PAGE_CACHEWRITEPOLICY	0x00000008
#define	PAGE_CAN_CACHE			0x00000010
#define	PAGE_ACCESSED			0x00000020
#define	PAGE_DIRTY				0x00000040
#define	PAGE_GLOBAL				0x00000100
#define	PAGE_ADDRESS			0xFFFFF000

//type representing a page table entry. just a typedef for an unsigned long
typedef uint32_t ptentry;

typedef struct
{
	ptentry pgtable_entries[PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));
} pgtable;

#endif
