#include <mm/paging.h>

pgdir main_pg_dir;
pgtable main_pg_table;

int switch_pg_dir(pgdir *__page_directory)
{
	if(!__page_directory)
		return -1;
		
	__asm__ __volatile__ ("movl %0, %%eax\n\t"
						  "movl %%eax, %%cr3"
						  ::"r"(__page_directory));
	
	return 0;
}

__inline int set_pt_entry(pgtable *__pgtable, unsigned int __pt_index, ptentry __entry)
{
	if(__pt_index >= PAGE_TABLE_ENTRIES)
		return -1;
	
	(*__pgtable).pgtable_entries[__pt_index] = __entry;
	
	return 0;
}

__inline int set_pgd_entry(pgdir *__pgdir, unsigned int __pgd_index, pdentry __entry)
{
	if(__pgd_index >= PAGE_DIR_ENTRIES)
		return -1;
		
	(*__pgdir).pgdir_entries[__pgd_index] = __entry;
	
	return 0;
}

void paging_startup()
{
	unsigned int idx;

	for(idx = 0;idx < PAGE_DIR_ENTRIES; idx++)
		set_pgd_entry(&main_pg_dir, idx, 0x2);
	
	for(idx = 0;idx < PAGE_TABLE_ENTRIES; idx++)
		set_pt_entry(&main_pg_table, idx, (ptentry)(idx*0x1000 | 0x3));
	
	set_pgd_entry(&main_pg_dir, 0, (pdentry)(((uint32_t)&main_pg_table) | 0x3));
	
	switch_pg_dir(&main_pg_dir);
}
