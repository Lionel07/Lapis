#include <config.h>
#include <memory.h>
#include <pmm.h>

memory_section_t pre_static_mem_regions[3];

extern "C" uintptr_t kernel_start;
extern "C" uintptr_t kernel_end;
void init_static_memregions() {
    pre_static_mem_regions[0].type             = RESERVED;
    pre_static_mem_regions[0].start_address    = 0x00000000;
    pre_static_mem_regions[0].end_address      = 0x00100000;
    pre_static_mem_regions[0].read_only        = true;
    pre_static_mem_regions[0].vmm_mapped       = 0;

    pre_static_mem_regions[1].type             = KERNEL_CODE;
    pre_static_mem_regions[1].start_address    = (uintptr_t)&kernel_start;
    pre_static_mem_regions[1].end_address      = (uintptr_t)&kernel_end;
    pre_static_mem_regions[1].read_only        = true;
    pre_static_mem_regions[1].vmm_mapped       = 0;

    pre_static_mem_regions[2].type             = KERNEL_HEAP;
    pre_static_mem_regions[2].start_address    = (uintptr_t)&kernel_end;
    pre_static_mem_regions[2].end_address      = (uintptr_t)&kernel_end + 0x100000;
    pre_static_mem_regions[2].read_only        = false;
    pre_static_mem_regions[2].vmm_mapped       = 0;

    Kernel::PMM::register_memoryRegion((memory_section_t*)&pre_static_mem_regions[0]);
    Kernel::PMM::register_memoryRegion((memory_section_t*)&pre_static_mem_regions[1]);
    //Kernel::PMM::register_memoryRegion((memory_section_t*)&pre_static_mem_regions[2]);
}