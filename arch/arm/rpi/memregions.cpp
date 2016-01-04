#include <config.h>
#include <memory.h>
#include <pmm.h>

memory_section_t mem_regions[2];

extern "C" uintptr_t kernel_start;
extern "C" uintptr_t kernel_end;
void init_static_memregions() {
    mem_regions[0].type             = KERNEL_CODE;
    mem_regions[0].start_address    = (uintptr_t)&kernel_start;
    mem_regions[0].end_address      = (uintptr_t)&kernel_end;
    mem_regions[0].read_only        = true;
    mem_regions[0].vmm_mapped       = 0;

    mem_regions[1].type             = RESERVED;
    mem_regions[1].start_address    = (uintptr_t)&kernel_end;
    mem_regions[1].end_address      = 0x00100000;
    mem_regions[1].read_only        = true;
    mem_regions[1].vmm_mapped       = 0;
    Kernel::PMM::register_memoryRegion((memory_section_t*)&mem_regions[0]);
    Kernel::PMM::register_memoryRegion((memory_section_t*)&mem_regions[1]);
}