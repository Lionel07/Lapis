#include <vmm.h>
#include <log/printk.h>
#include <string.h>
#include <config.h>
#include <memory.h>

void Kernel::VMM::init()
{
    printk(LOG_INFO, "vmm: Initialsing Virtual Memory Manager...\n");
    Kernel::VMM::arch_init();
    printk(LOG_INFO, "vmm: Arch init finished.\n");
    // Idealy, we should identity map where the kernel is.
}

void Kernel::VMM::allocatePage(vmm_page_t page)
{

}