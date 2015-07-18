#include <pmm.h>
#include <log/printk.h>

#define BUDDY_BITMAP_SIZE_MAGIC 12

#define PMM_PRINTDEBUGSTATEMENTS true

// TODO(Lionel07): A math.h
int ipow(int base, int exp);

uintptr_t   Kernel::PMM::memsize;
uintptr_t   Kernel::PMM::buddy_usedPages;
uintptr_t * Kernel::PMM::buddy_startPage;
uintptr_t   Kernel::PMM::kernel_allocatedPages;
uintptr_t   Kernel::PMM::kernel_uncommitedAllocatedPages;

void Kernel::PMM::init() {
    printk(LOG_INFO, "pmm: Initialsing Physical Memory Manager...\n");

    memsize = 0xF000000;    // Get memsize here.
    buddy_usedPages = 0;

    for (int i = 0; i != BUDDY_BITMAPS; i++) {
        if (PMM_PRINTDEBUGSTATEMENTS) {
            printk(LOG_INFO, "pmm: Creating buddy bitmap %d, resolution 0x%X, size 0x%X bytes\n", \
             i, ipow(2, BUDDY_BITMAP_SIZE_MAGIC + i), memsize / ipow(2, BUDDY_BITMAP_SIZE_MAGIC + i));
        }
        int needed_size = memsize / ipow(2, BUDDY_BITMAP_SIZE_MAGIC + i);
        if ((needed_size & 0xFFFFF000) != needed_size) {
            needed_size &= 0xFFFFF000;
            needed_size += 0x1000;
        }
        buddy_usedPages+=needed_size / 0x1000;
    }
    printk(LOG_INFO, "pmm: Requires 0x%X pages for bookkeeping\n", buddy_usedPages);
    kernel_uncommitedAllocatedPages += buddy_usedPages;
}
