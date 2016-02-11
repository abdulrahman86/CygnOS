#include <pmm.h>
#include <string.h>
#include <mm/paging.h>

__inline pgdir *get_cur_pgdir()
{
	uint32_t cr3_val;
	
	__asm__ __volatile__ ("movl %%cr3, %%eax"
						  :"=a"(cr3_val));

	return (pgdir *)cr3_val;
}

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

ptentry *get_ptentry_from_vaddr(pgtable *__pgtable, virtaddr __addr)
{
	if(__pgtable)
	{
		int pgtable_index = __PAGE_TABLE_INDEX(__addr);
		
		if(pgtable_index >= PAGE_TABLE_ENTRIES)
			return 0;
		else
			return &(*__pgtable).pgtable_entries[pgtable_index];
	}
		
	return 0;
}

pdentry *get_pdentry_from_vaddr(pgdir *__pgdir, virtaddr __addr)
{
	if(__pgdir)
	{
		int pgdir_index = __PAGE_DIRECTORY_INDEX(__addr);
		
		if(pgdir_index >= PAGE_DIR_ENTRIES)
			return 0;
		else
			return &(*__pgdir).pgdir_entries[pgdir_index];
	}
	
	return 0;
}

void paging_startup()
{
	unsigned int idx;
	
	pgdir *startup_pgdir = (pgdir *)pmm_alloc();

	for(idx = 0;idx < PAGE_DIR_ENTRIES; idx++)
		set_pgd_entry(startup_pgdir, idx, 0x2);
		
	pgtable *startup_pgtable = (pgtable *)pmm_alloc();
	
	for(idx = 0;idx < PAGE_TABLE_ENTRIES; idx++)
		set_pt_entry(startup_pgtable, idx, (ptentry)(idx*0x1000 | PAGE_PRESENT | PAGE_RW));
	
	set_pgd_entry(startup_pgdir, 0, (pdentry)(((uint32_t)startup_pgtable) | PAGE_PRESENT | PAGE_RW));
	
	switch_pg_dir(startup_pgdir);
}

int alloc_page(ptentry *__entry)
{
	uint32_t page_addr = pmm_alloc();
	
	if(!page_addr)
		return -1;
	
	*__entry = (ptentry)(page_addr | PAGE_PRESENT);
	
	return 0;
}

int dealloc_page(ptentry *__entry)
{
	uint32_t page_addr = (*__entry) & PAGE_ADDRESS;
	
	int dealloc_ret = pmm_dealloc(page_addr);
	
	*__entry &= !PAGE_PRESENT;
	
	return dealloc_ret;
}

int map_phys_to_virt(uint32_t __paddr, virtaddr __vaddr)
{
	pgdir *cur_pgdir = get_cur_pgdir();
	
	pdentry pgdir_entry = (*cur_pgdir).pgdir_entries[__PAGE_DIRECTORY_INDEX(__vaddr)];
	if((pgdir_entry & PGTAB_PRESENT) != PGTAB_PRESENT)
	{
		pgtable *new_pgtable = (pgtable *)pmm_alloc();
		
		if(new_pgtable == 0)
			return -1;
			
		memset((void *)new_pgtable, 0, sizeof(pgtable));
		
		pgdir_entry = (pdentry)(((uint32_t)new_pgtable) | PAGE_PRESENT | PAGE_RW);
		set_pgd_entry(cur_pgdir, __PAGE_DIRECTORY_INDEX(__vaddr), pgdir_entry);
	}
	
	ptentry page_ptentry = (ptentry)((__paddr & PAGE_ADDRESS) | PAGE_PRESENT | PAGE_RW);
	set_pt_entry((pgtable *)(pgdir_entry & PGTAB_TABLE_ADDRESS), __PAGE_TABLE_INDEX(__vaddr), page_ptentry);
	
	return 0;
}
