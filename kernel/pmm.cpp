#include <pmm.h>
#include <log/printk.h>
#include <string.h>

#define PMM_PRINTDEBUGSTATEMENTS true

// TODO(Lionel07): A math.h
int ipow(int base, int exp);

uintptr_t   Kernel::PMM::memsize;
uintptr_t   Kernel::PMM::buddy_usedPages;
uintptr_t * Kernel::PMM::buddy_startPage[BUDDY_BITMAPS];
uintptr_t   Kernel::PMM::kernel_allocatedPages;
uintptr_t   Kernel::PMM::kernel_uncommitedAllocatedPages;

void Kernel::PMM::init() {
    printk(LOG_INFO, "pmm: Initialsing Physical Memory Manager...\n");

    memsize = 0xF000000;    // TODO(Lionel07): Get memsize here.
    buddy_usedPages = 0;

    uintptr_t start_of_allocatable_space = 0x1000000;
    uintptr_t * buddyAllocatorPointer = (uintptr_t*) start_of_allocatable_space;

    for (int i = 0; i != BUDDY_BITMAPS; i++) {
        int needed_size = memsize / ipow(2, BUDDY_STARTSIZE + i);
        if ((needed_size & 0xFFFFF000) != needed_size) {
            needed_size &= 0xFFFFF000;
            needed_size += 0x1000;
        }

        memset((void*)buddyAllocatorPointer, 0, needed_size);

        buddy_startPage[i]      = buddyAllocatorPointer;
        buddy_usedPages        += needed_size / 0x1000;
        buddyAllocatorPointer  += needed_size;

        if (PMM_PRINTDEBUGSTATEMENTS) {
            printk(LOG_INFO, "pmm: Created bitmap %d, resolution 0x%X, size 0x%X bytes\n", \
             i, ipow(2, BUDDY_STARTSIZE + i), needed_size);
            printk(LOG_INFO, "pmm: @0x%X\n", (uintptr_t)buddy_startPage[i]);
        }
    }
    printk(LOG_INFO, "pmm: Requires 0x%X pages for bookkeeping\n", buddy_usedPages);
    kernel_uncommitedAllocatedPages += buddy_usedPages;

    // Allocate pages here
}
