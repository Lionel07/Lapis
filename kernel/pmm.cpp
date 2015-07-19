#include <pmm.h>
#include <log/printk.h>
#include <string.h>

/// Should we print debug statements?
#define PMM_PRINTDEBUGSTATEMENTS true
/// Page index from address
#define INDEX_FROM_BIT(b) (b / 0x20)
/// Page offest from address
#define OFFSET_FROM_BIT(b) (b % 0x20)

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
    for (uintptr_t i = 0; i < start_of_allocatable_space; i+=0x1000) {
        buddy_allocatePage(i);
    }
}
int Kernel::PMM::buddy_allocatePage(uintptr_t address) {
    // printk(LOG_DEBUG, "pmm buddy: Allocating page 0x%X\n", address);

    uintptr_t frame_addr = address / 0x1000;
    uintptr_t index = INDEX_FROM_BIT(frame_addr);
    uintptr_t offset = OFFSET_FROM_BIT(frame_addr);

    uintptr_t *bitmap = buddy_startPage[0];
    bitmap[index] |= (0x1 << offset);
    // Update higher bitmaps by setting the bits to ON
    for (int i = 0; i!= BUDDY_BITMAPS; i++) {
        break;  // TODO(Lionel07): Fix this.
        if (bitmap[index] == (uintptr_t)-1) {
        }
    }
    return 0;
}
int Kernel::PMM::buddy_freePage(uintptr_t address) {
    // printk(LOG_DEBUG, "pmm buddy: Freeing page 0x%X\n", address);

    uintptr_t frame_addr = address / 0x1000;
    uintptr_t index = INDEX_FROM_BIT(frame_addr);
    uintptr_t offset = OFFSET_FROM_BIT(frame_addr);

    uintptr_t *bitmap = buddy_startPage[0];
    bitmap[index] &= ~(0x1 << offset);
    // Update higher bitmaps by setting the bits to ON
    for (int i = 0; i!= BUDDY_BITMAPS; i++) {
        break;  // TODO(Lionel07): Fix this.
        if (bitmap[index] == (uintptr_t)-1) {
        }
    }
    return 0;
}
