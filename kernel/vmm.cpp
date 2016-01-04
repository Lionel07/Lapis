#include <vmm.h>
#include <log/printk.h>
#include <string.h>
#include <config.h>
#include <memory.h>
#include <bits.h>
void Kernel::VMM::init()
{
    printk(LOG_INFO, "vmm: Initialsing Virtual Memory Manager...\n");
    Kernel::VMM::arch_init();
    printk(LOG_INFO, "vmm: Arch init finished.\n");
    // Idealy, we should identity map where the kernel is.

    

}

void Kernel::VMM::allocatePage(vmm_page_t page)
{
	if(CHECK_BIT(page.flags,VMM_FLAG_ALLOCATED) || CHECK_BIT(page.flags,VMM_FLAG_PRESENT))
	{
		printk(LOG_ERR, "vmm alloc: Attempted to re-allocate a page\n");
		return;
	}
}

void Kernel::VMM::printVmmPage(vmm_page_t page)
{
	// Note: Since all values are 32bit, x86_page works.
	printk(LOG_DEBUG, "vmm debug: Page 0x%X @ 0x%X ( ",page.address.x86_page, page.phys_address.x86_page); 
	if(CHECK_BIT(page.flags,VMM_FLAG_ALLOCATED))
	{
		printk(LOG_DEBUG, "alloc ");
	}
	if(CHECK_BIT(page.flags,VMM_FLAG_PRESENT))
	{
		printk(LOG_DEBUG, "present ");
	}
	if(CHECK_BIT(page.flags,VMM_FLAG_RW))
	{
		printk(LOG_DEBUG, "rw ");
	}
	if(CHECK_BIT(page.flags,VMM_FLAG_KERNEL))
	{
		printk(LOG_DEBUG, "kernel ");
	}
	if(CHECK_BIT(page.flags,VMM_FLAG_DIRTY))
	{
		printk(LOG_DEBUG, "dirty ");
	}
	printk(LOG_DEBUG, ")\n");
}