#include <gdt.h>

void gdt_set_default()
{
	//set null descriptor
	gdt_default[0].limit_low = 0;
	gdt_default[0].base_low  = 0;
	gdt_default[0].base_mid  = 0;
	gdt_default[0].flags	 = 0;
	gdt_default[0].base_high = 0;
	
	//set code descriptor
	gdt_default[1].limit_low = 0xFFFF;
	gdt_default[1].base_low  = 0;
	gdt_default[1].base_mid  = 0;
	gdt_default[1].flags     = I686_GDT_4K_GRAN | I686_GDT_32_BIT_SEG | I686_GDT_OS_RESERVED | I686_GDT_RESERVED |
							   0xF |
							   I686_GDT_MEM_SEGMENT | I686_GDT_RING_0 | I686_GDT_CODE_DATA_DESCR |
							   I686_GDT_CODE_SEG | I686_GDT_CONFORMING | I686_GDT_RW_RE;
	gdt_default[1].base_high = 0;
	
	//set data descriptor
	gdt_default[2].limit_low = 0xFFFF;
	gdt_default[2].base_low  = 0;
	gdt_default[2].base_mid  = 0;
	gdt_default[2].flags     = I686_GDT_4K_GRAN | I686_GDT_32_BIT_SEG | I686_GDT_OS_RESERVED | I686_GDT_RESERVED |
							   0xF |
							   I686_GDT_MEM_SEGMENT | I686_GDT_RING_0 | I686_GDT_CODE_DATA_DESCR |
							   I686_GDT_DATA_SEG | I686_GDT_CONFORMING | I686_GDT_RW_RE;;
	gdt_default[2].base_high = 0;
	
	//set data to be loaded into GDTR
	gdtr_default.gdt_limit = DEFAULT_GDT_MAX_DESCRIPTORS*sizeof(i686_gdt_descriptor) - 1;
	gdtr_default.gdt_base  = (uintptr_t)&gdt_default[0];
	
	//load GDTR and the GDT
	_i686_gdt_install();
}
