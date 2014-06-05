#ifndef __GDT_HEADER
#define	__GDT_HEADER

#include <stdint.h>

#define DEFAULT_GDT_MAX_DESCRIPTORS	3

#define	I686_GDT_ACCESS			0x1

#define	I686_GDT_SEGMENT_RE		0x0
#define	I686_GDT_SEGMENT_RW_RE		0x0002

#define	I686_GDT_SEGMENT_CONFORMING	0x0
#define	I686_GDT_SEGMENT_EXPNSN_DIR	0x0004

#define	I686_GDT_SEGMENT_DATA		0x0
#define	I686_GDT_SEGMENT_CODE		0x0008

#define	I686_GDT_DESCRIPTOR_SYSTEM	0x0
#define	I686_GDT_DESCRIPTOR_CODE_DATA	0x0010

#define	I686_GDT_RING_0			0x0
#define	I686_GDT_RING_3			0x0060

#define	I686_GDT_SEGMENT_VM		0x0
#define	I686_GDT_SEGMENT_MEMORY		0x0080

#define	I686_GDT_RESERVED		0x0

#define	I686_GDT_OS_RESERVED		0x0

#define	I686_GDT_SEGMENT_16		0x0
#define	I686_GDT_SEGMENT_32		0x4000

#define	I686_GDT_GRANULARITY_1B		0x0
#define	I686_GDT_GRANULARITY_4K		0x8000

typedef struct
{
	//low[0-15] 16 bits of segment's limit
	uint16_t	limit_low;	
	
	//low[0-15] 16 bits of segment's base
	uint16_t	base_low;	
	
	//mid[16-23] 8 bits of segment's base
	uint8_t		base_mid;	
	
	uint16_t	flags;		
	
	//high[24-31] bits of segment's base
	uint8_t		base_high;	
} i686_gdt_descriptor;

typedef struct __attribute__((packed))
{
	//size of the GDT - 1
	uint16_t	gdt_limit;
	
	//base address of the GDT
	uintptr_t	gdt_base;
} i686_gdtr_data;

//structure instance holding GDTR data for the default GDT
i686_gdtr_data gdtr_default;

//set up and install the default GDT
void gdt_set_default();

//actually loads data into GDTR, implemented in assembly
extern void _i686_gdt_install();
#endif
