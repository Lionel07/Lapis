#include <stdint.h>
#include <arch/x86/paging.h>
#include <log/printk.h>
#include <string.h>
#include <pmm.h>
#include <vmm.h>
#include <panic.h>
#include <arch/x86/idt.h>
#include <arch/x86/irq.h>
#include <bits.h>
// x86 bits of VMM

uintptr_t * page_directory;

uintptr_t * page_tables_addr[1024];

void Kernel::VMM::arch_init()
{
    printk(LOG_INFO, "vmm: Initialsing Paging...\n");
    // Idealy, we should identity map where the kernel is.

    // Page directory is only 1kb, so we can just grab a free page.

    memset(page_tables_addr,0,0x1000);

    page_directory = Kernel::PMM::allocateSinglePage();
    memset(page_directory,0,0x1000);
    printk(LOG_DEBUG, "vmm: Created Page Directory @ 0x%X\n",(uintptr_t)page_directory);

    // Let's clear the page directory

    for (uintptr_t i = 0; i < 1024; i++)
    {
    	page_directory[i] = 0x00000002;
    }

    // TODO, make more modular
    arch_allocateTable(0);
    // Manually identity map the table for now.

    for(int i = 0; i < 1024; i++)
    {
    	uintptr_t * table = page_tables_addr[0];
    	table[i] = (i * 0x1000) | 3; // Set VMM_FLAG_KERNEL, VMM_FLAG_RW, and VMM_FLAG_PRESENT
    }

    // Now we need to enable paging manually.

    asm volatile("mov %0, %%cr3":: "r"(page_directory));
    uint32_t cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0":: "r"(cr0));

}
void Kernel::VMM::arch_allocateTable(uintptr_t idx)
{
	uintptr_t * table = Kernel::PMM::allocateSinglePage();
	memset(table,0,0x1000);
	page_tables_addr[idx] = table;

	// Add to page directory
	page_directory[idx] = (uintptr_t)table | 3;
}
void Kernel::VMM::arch_freeTable(uintptr_t idx)
{
	page_directory[idx] = 0x00000002; // Set NOT_PRESENT
	page_tables_addr[idx] = 0;
}

void Kernel::VMM::arch_identityMap(uintptr_t start, uintptr_t end)
{
	int page_table_index = (start / 0x1000) / 0x1000;
	if(page_tables_addr[page_table_index] == 0)
	{
		printk(LOG_PANIC, "vmm: Attempted to identity map an empty address (unimplemented) 0x%X\n",page_table_index);
		panic("Page table did not exist");
		return;
	}

	if( end <= start )
	{
		panic("vmm identity map: end < start");
		return;
	}

	uintptr_t current_address = start;
	uintptr_t * table = page_tables_addr[page_table_index];
	//TODO: Allow jumping page boundaries.
	//TODO: Do not assume we start at 0

	uintptr_t index = 0;

	while (current_address < end)
	{
		table[index] = (current_address) | 3;
		current_address += 0x1000;
	}
}

void Kernel::VMM::arch_allocatePage(vmm_page_t page)
{
	//We've been given a page, now we need to decode it

	uintptr_t address = page.address.x86_page;

	if(CHECK_BIT(page.flags,VMM_FLAG_ALLOCATED) || CHECK_BIT(page.flags,VMM_FLAG_PRESENT))
	{
		printk(LOG_ERR, "vmm alloc: Attempted to re-allocate page 0x%X (mapped at 0x%X)\n",
			page.address.x86_page,page.phys_address.x86_page);
		return;
	}

}

vmm_page_t Kernel::VMM::addressToVmmPage(uintptr_t page)
{
	vmm_page_t a;
	//memset(&a, 0, sizeof(vmm_page_t));
	a.address.x86_page = page;

	// Get info.
	uintptr_t address_index = page / 0x1000;
	uintptr_t table_index = address_index / 1024;
	// Now every frame can be addressed as [table_index:address]

	if(page_tables_addr[table_index] == 0)
	{

	}

	return a;
}
uintptr_t Kernel::VMM::VmmPageToAddress(vmm_page_t page)
{
	return page.address.x86_page;
}