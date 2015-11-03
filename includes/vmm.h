#ifndef KERNEL_MEMORY_VMM_H
#define KERNEL_MEMORY_VMM_H
#include <stddef.h>
#include <stdint.h>
#include <memory.h>
#include <config.h>

typedef union vmm_page_addr {
	uint32_t x86_page;
	uint32_t arm_page;
} vmm_page_address_t;


typedef struct vmm_page
{
	vmm_page_address_t address;
	vmm_page_address_t phys_address;
	uint8_t flags;
} vmm_page_t;

#define VMM_FLAG_ALLOCATED  0
#define VMM_FLAG_RW 		1
#define VMM_FLAG_PRESENT 	2
#define VMM_FLAG_KERNEL 	3
#define VMM_FLAG_DIRTY 		4	// For CoW

namespace Kernel {
    class VMM {
    public:
    public:
        static void init();
        static void arch_init();

        static void arch_allocatePage(vmm_page_t page);
        static void arch_freePage(vmm_page_t page);
        static void arch_mapPage(uintptr_t *phys, uintptr_t *virt, vmm_page_t page);
        static void arch_unmapPage(uintptr_t *phys, uintptr_t *virt, vmm_page_t page);
        static void arch_allocateTable(uintptr_t idx);
        static void arch_freeTable(uintptr_t idx);

        static void arch_identityMap(uintptr_t start, uintptr_t end);

        static void allocatePage(vmm_page_t page);
        static void freePage(vmm_page_t page);
        static void mapPage(uintptr_t *phys, uintptr_t *virt, vmm_page_t page);
        static void unmapPage(uintptr_t *phys, uintptr_t *virt, vmm_page_t page);
        static vmm_page_t addressToVmmPage(uintptr_t page);
        static uintptr_t VmmPageToAddress(vmm_page_t page);
        static void printVmmPage(vmm_page_t page);
        static void flushPage(vmm_page_t page);
    };
};

#endif
