#ifndef	__PAGING_HEADER
#define	__PAGING_HEADER

#include "pgtable.h"
#include "pgdir.h"

//enable paging
extern void _i686_enable_paging();

//function to change the currently used page directory, effectively clering the TLB and changing the currently used page tables
extern int switch_pg_dir(pgdir *);

//sets entry number __pt_index of page table pointed to by __pgtable to the value __entry
extern int set_pt_entry(pgtable *__pgtable, unsigned int __pt_index, ptentry __entry);

//sets entry number __pgd_index of page directory pointed to by __pgdir to the value __entry
extern int set_pgd_entry(pgdir *__pgdir, unsigned int __pgd_index, pdentry __entry);

//routine which sets up paging for use during kernel start-up. this set-up will be destroyed once the initial working environment has been set up properly
extern void paging_startup();

#endif
