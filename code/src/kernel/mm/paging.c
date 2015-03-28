#include <mm/paging.h>

int switch_pg_dir(pgdir *__page_directory)
{
	if(!__page_directory)
		return -1;
		
	__asm__ __volatile__ ("movl %%eax, %0\n\t"
						  "movl %%cr3, %%eax"
						  ::"r"(__page_directory));
	
	return 0;
}
