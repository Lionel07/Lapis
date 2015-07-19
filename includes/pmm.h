#ifndef KERNEL_MEMORY_PMM_H
#define KERNEL_MEMORY_PMM_H
#include <stddef.h>
#include <stdint.h>

#define BUDDY_BITMAPS 2
#define BUDDY_STARTSIZE 12

namespace Kernel {
    class PMM {
    public:
        static uintptr_t memsize;
        static uintptr_t buddy_usedPages;
        static uintptr_t * buddy_startPage[BUDDY_BITMAPS];
        static uintptr_t kernel_allocatedPages;
        static uintptr_t kernel_uncommitedAllocatedPages;
    public:
        static void refreshCache();
        static void init();
        static int allocate(int pages);
        static int buddy_allocatePage(uintptr_t address);
        static int free(uintptr_t * address);
    };
};

#endif
