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

//the (for now) single page directory used throughout the system
extern pgdir main_pg_dir;

//typedef for a virtual address
typedef uint32_t virtaddr;

//get a new page from the physical memory manager and assign it to the given page table entry
//the present flag will be set for the page
extern int alloc_page(ptentry *__entry);

//return the page assigned to the given page table entry to the physical memory manager
//the present flag for the entry will be cleared
extern int dealloc_page(ptentry *__entry);

//returns address of the page table entry corresponding to the given virtual address in the given page table
extern ptentry *get_ptentry_from_vaddr(pgtable *__pgtable, virtaddr __addr);

//returns address of the page directory entry corresponding to the given virtual address in the given page directory
extern pdentry *get_pdentry_from_vaddr(pgdir *__pgdir, virtaddr __addr);

#endif
