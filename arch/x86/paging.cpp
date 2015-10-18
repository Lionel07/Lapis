#include <stdint.h>
#include <arch/x86/paging.h>
#include <log/printk.h>
#include <string.h>
#include <pmm.h>
#include <vmm.h>
#include <panic.h>
#include <arch/x86/idt.h>
#include <arch/x86/irq.h>

// x86 bits of VMM

void Kernel::VMM::arch_init()
{
    printk(LOG_INFO, "vmm arch: Initialsing Paging...\n");
    // Idealy, we should identity map where the kernel is.
}

void Kernel::VMM::arch_allocatePage(vmm_page_t page)
{

}

vmm_page_t Kernel::VMM::addressToVmmPage(uintptr_t page)
{
	vmm_page_t a;
	return a;
}
uintptr_t Kernel::VMM::VmmPageToAddress(vmm_page_t page)
{
	return 0;
}