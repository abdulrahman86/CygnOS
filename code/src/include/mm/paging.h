#ifndef	__PAGING_HEADER
#define	__PAGING_HEADER

#include "pgdir.h"

//enable paging
extern void _i686_enable_paging();

//function to change the currently used page directory, effectively clering the TLB and changing the currently used page tables
extern int switch_pg_dir(pgdir *);

#endif
