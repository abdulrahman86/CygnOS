#include <mm/paging.h>

pgdir kernel_pg_dir;
pgtable kernel_pg_table;

int switch_pg_dir(pgdir *__page_directory)
{
	if(!__page_directory)
		return -1;
		
	__asm__ __volatile__ ("movl %%eax, %0\n\t"
						  "movl %%cr3, %%eax"
						  ::"r"(__page_directory));
	
	return 0;
}

int __inline set_pt_entry(pgtable *__pgtable, unsigned int __pt_index, ptentry __entry)
{
	if(__pt_index >= PAGE_TABLE_ENTRIES)
		return -1;
	
	(*__pgtable).pgtable_entries[__pt_index] = __entry;
	
	return 0;
}

int __inline set_pgd_entry(pgdir *__pgdir, unsigned int __pgd_index, pdentry __entry)
{
	if(__pgd_index >= PAGE_DIR_ENTRIES)
		return -1;
		
	(*__pgdir).pgdir_entries[__pgd_index] = __entry;
	
	return 0;
}

void paging_startup()
{
	unsigned int idx;
	
	for(idx = 0;idx < PAGE_TABLE_ENTRIES; idx++)
		set_pt_entry(&kernel_pg_table, idx, 0);
	
	for(idx = 0;idx < PAGE_DIR_ENTRIES; idx++)
		set_pgd_entry(&kernel_pg_dir, idx, 0);
}
