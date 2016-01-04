#include <stdint.h>
#include <log/printk.h>
#include <string.h>
#include <pmm.h>
#include <vmm.h>
#include <panic.h>
#include <bits.h>

void Kernel::VMM::arch_init()
{
    printk(LOG_INFO, "vmm: Initialsing VMM...\n");

}
void Kernel::VMM::arch_allocateTable(uintptr_t idx)
{

}
void Kernel::VMM::arch_freeTable(uintptr_t idx)
{

}

void Kernel::VMM::arch_identityMap(uintptr_t start, uintptr_t end)
{
}

void Kernel::VMM::arch_allocatePage(vmm_page_t page)
{
}


vmm_page_t Kernel::VMM::addressToVmmPage(uintptr_t page)
{
    vmm_page_t a;
    //memset(&a, 0, sizeof(vmm_page_t));
    a.address.arm_page = page;

    return a;
}
uintptr_t Kernel::VMM::VmmPageToAddress(vmm_page_t page)
{
    return page.address.arm_page;
}