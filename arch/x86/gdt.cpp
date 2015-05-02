#include <arch/x86/gdt.h>
#include <stdint.h>
#include <log/printk.h>
#include <stddef.h>

#define ARCH_X86_GDT_ENTRIES 5

struct gdt_entry 	arch_x86_gdt[ARCH_X86_GDT_ENTRIES];
struct gdt_ptr  	arch_x86_gdtptr;

void arch_x86_gdt_set_gate(signed int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	arch_x86_gdt[num].base_low    = (base & 0xFFFF);
	arch_x86_gdt[num].base_middle = (base >> 16) & 0xFF;
	arch_x86_gdt[num].base_high   = (base >> 24) & 0xFF;

	arch_x86_gdt[num].limit_low   = (limit & 0xFFFF);
	arch_x86_gdt[num].granularity = (limit >> 16) & 0x0F;

	arch_x86_gdt[num].granularity |= gran & 0xF0;
	arch_x86_gdt[num].access      = access;

	printk(LOG_DEBUG,"x86: 0x%x 0x%x 0x%x 0x%x 0x%x\n",num,base,limit,access,gran);
} 

void arch_x86_gdt_init() {
	printk(LOG_INFO,"x86: Writing GDT to hardware\n");
	arch_x86_gdtptr.limit = (sizeof(struct gdt_entry) * ARCH_X86_GDT_ENTRIES) - 1;
	arch_x86_gdtptr.base  = (uint32_t)&arch_x86_gdt;
	printk(LOG_DEBUG,"x86: #NO        BASE       LIMIT      ACCESS     GRAN      \n");
	arch_x86_gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
	arch_x86_gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	arch_x86_gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	arch_x86_gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	arch_x86_gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
	gdt_flush((uint32_t)&arch_x86_gdtptr);
	printk(LOG_INFO,"x86: Done\n");
}