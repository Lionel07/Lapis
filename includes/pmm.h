#ifndef KERNEL_MEMORY_PMM_H
#define KERNEL_MEMORY_PMM_H
#include <stddef.h>
#include <stdint.h>
#include <config.h>
namespace Kernel {
    class PMM {
    public:
        static uintptr_t memsize;
        static uintptr_t buddy_usedPages;
        static uintptr_t * buddy_startPage[PMM_BUDDY_BITMAPS];
        static uintptr_t kernel_allocatedPages;
        static uintptr_t kernel_uncommitedAllocatedPages;
    public:
        static void        refreshCache();
        static void        init();
        static uintptr_t * allocate(unsigned int pages);
        static uintptr_t   buddy_allocatePage(uintptr_t address);
        static uintptr_t   buddy_freePage(uintptr_t address);
        static uintptr_t   buddy_testPage(uintptr_t address);
        static uintptr_t   buddy_getFirstPage();
        static uintptr_t   free(uintptr_t * address);
        static uintptr_t   shim_allocatePage(uintptr_t address);
        static uintptr_t   shim_deallocatePage(uintptr_t address);
    };
};

#endif
