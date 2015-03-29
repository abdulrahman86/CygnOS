#ifndef	__PGDIR_HEADER
#define	__PGDIR_HEADER

#include <stdint.h>

#define	PAGE_DIR_ENTRIES		1024

//bit masks for the different bits of a 32-bit non-PAE page directory entry
#define PGTAB_PRESENT            0x00000001
#define PGTAB_RW                 0x00000002
#define PGTAB_PRIVILEGE          0x00000004
#define PGTAB_CACHEWRITEPOLICY   0x00000008
#define PGTAB_CAN_CACHE          0x00000010
#define PGTAB_ACCESSED           0x00000020
#define PGTAB_LARGE              0x00000080
#define PGTAB_GLOBAL             0x00000100
#define PGTAB_TABLE_ADDRESS      0xFFFFF000

//type representing a page directory entry. just a typedef for an unsigned long
typedef unsigned long pdentry;

typedef struct
{
	pdentry pgdir_entries[PAGE_DIR_ENTRIES];
} pgdir;

#endif
