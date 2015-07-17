#include <pmm.h>
#include <log/printk.h>

// TODO(Lionel07): A math.h
int ipow(int base, int exp);

uintptr_t   Kernel::PMM::memsize;
uintptr_t   Kernel::PMM::buddy_usedpages;
uintptr_t * Kernel::PMM::buddy_startpage;


void Kernel::PMM::init() {
    printk(LOG_INFO, "pmm: Initialsing Physical Memory Manager...\n");

    memsize = 0xF000000;    // Get memsize here.
    buddy_usedpages = 0;

    for (int i = 0; i != BUDDY_BITMAPS; i++) {
        printk(LOG_INFO, "pmm: Creating buddy bitmap %d, resolution 0x%X, size 0x%X bytes\n", i, ipow(2, 12 + i), memsize / ipow(2, 12 + i));
        int needed_size = memsize / ipow(2, 12 + i);
        if ((needed_size & 0xFFFFF000) != needed_size) {
            needed_size &= 0xFFFFF000;
            needed_size += 0x1000;
        }
        buddy_usedpages+=needed_size / 0x1000;
    }
    printk(LOG_INFO, "pmm: Requires 0x%X pages for bookkeeping\n", buddy_usedpages);
}
