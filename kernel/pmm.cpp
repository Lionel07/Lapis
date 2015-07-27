#include <pmm.h>
#include <log/printk.h>
#include <string.h>
#include <config.h>
#include <memory.h>
/// Should we print debug statements?

/// Page index from address
#define INDEX_FROM_BIT(b) (b / 0x20)
/// Page offest from address
#define OFFSET_FROM_BIT(b) (b % 0x20)

// TODO(Lionel07): A math.h
int ipow(int base, int exp);

uintptr_t   Kernel::PMM::memsize;
uintptr_t   Kernel::PMM::buddy_usedPages;
uintptr_t * Kernel::PMM::buddy_startPage[PMM_BUDDY_BITMAPS];
uintptr_t   Kernel::PMM::kernel_allocatedPages;
uintptr_t   Kernel::PMM::kernel_uncommitedAllocatedPages;
uintptr_t   Kernel::PMM::kernel_totalPages;
uintptr_t   Kernel::PMM::kernel_usedSectionSlots = 0;

memory_section_t * Kernel::PMM::section_slots[MEMORY_SECTION_SLOTS];

void Kernel::PMM::init() {
    printk(LOG_INFO, "pmm: Initialsing Physical Memory Manager...\n");

    memsize = 0xF000000;    // TODO(Lionel07): Get memsize here.
    buddy_usedPages = 0;
    kernel_totalPages = memsize / 0x1000;
    uintptr_t start_of_allocatable_space = 0x1000000;
    uintptr_t * buddyAllocatorPointer = (uintptr_t*) start_of_allocatable_space;

    for (int i = 0; i != PMM_BUDDY_BITMAPS; i++) {
        uintptr_t needed_size = memsize / ipow(2, PMM_BUDDY_STARTSIZE + i);
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
             i, ipow(2, PMM_BUDDY_STARTSIZE + i), needed_size);
            //printk(LOG_INFO, "pmm: @0x%X\n", (uintptr_t)buddy_startPage[i]);
        }
    }

    printk(LOG_INFO, "pmm: Requires 0x%X pages for bookkeeping\n", buddy_usedPages);
    kernel_uncommitedAllocatedPages += buddy_usedPages;

    // Allocate pages here
    for (uintptr_t i = 0; i < start_of_allocatable_space; i+=0x1000) {
        buddy_allocatePage(i);
    }
}
void Kernel::PMM::debugPrintStatistics() {
     printk(LOG_DEBUG, "pmm stat: memsize 0x%X, alloc. pages 0x%X, uncommited 0x%X\n", memsize,kernel_allocatedPages,kernel_uncommitedAllocatedPages);
     printk(LOG_DEBUG, "pmm stat: buddy pages [0]0x%X, [1]0x%X, ..., total %d\n", buddy_startPage[0],buddy_startPage[1], PMM_BUDDY_BITMAPS);
}
uintptr_t Kernel::PMM::buddy_allocatePage(uintptr_t address) {
    // printk(LOG_DEBUG, "pmm buddy: Allocating page 0x%X\n", address);

    uintptr_t frame_addr = address / 0x1000;
    uintptr_t index = INDEX_FROM_BIT(frame_addr);
    uintptr_t offset = OFFSET_FROM_BIT(frame_addr);

    uintptr_t *bitmap = buddy_startPage[0];
    bitmap[index] |= (0x1 << offset);
    // Update higher bitmaps by setting the bits to ON
    for (int i = 0; i!= PMM_BUDDY_BITMAPS; i++) {
        break;  // TODO(Lionel07): Fix this.
        if (bitmap[index] == (uintptr_t)-1) {
        }
    }
    kernel_allocatedPages++;
    return 0;
}
uintptr_t Kernel::PMM::buddy_freePage(uintptr_t address) {
    // printk(LOG_DEBUG, "pmm buddy: Freeing page 0x%X\n", address);

    uintptr_t frame_addr = address / 0x1000;
    uintptr_t index = INDEX_FROM_BIT(frame_addr);
    uintptr_t offset = OFFSET_FROM_BIT(frame_addr);

    uintptr_t *bitmap = buddy_startPage[0];
    bitmap[index] &= ~(0x1 << offset);
    // Update higher bitmaps by setting the bits to ON
    for (int i = 0; i!= PMM_BUDDY_BITMAPS; i++) {
        break;  // TODO(Lionel07): Fix this.
        if (bitmap[index] == (uintptr_t)-1) {
        }
    }
    return 0;
}

uintptr_t Kernel::PMM::buddy_testPage(uintptr_t address) {
    // TODO(Lionel07): Optimize this by utilizing the buddy cache layers.
    uintptr_t *bitmap = buddy_startPage[0];

    uintptr_t frame_addr = address / 0x1000;
    uintptr_t index = INDEX_FROM_BIT(frame_addr);
    uintptr_t offset = OFFSET_FROM_BIT(frame_addr);
    return (bitmap[index] & (0x1 << offset));
}

uintptr_t * Kernel::PMM::allocate(unsigned int pages) {
    if (pages < 1) {
        return 0;
    }
    uintptr_t potential_first = buddy_getFirstPage() * 0x1000;
    for (unsigned int i = 0; i != pages; i++) {
        if (buddy_testPage(potential_first + (0x1000 * i)) == 1) {
            potential_first = (potential_first + (0x1000 * i));
        }
    }

    // It passes, keep potential first and return it after setting used bits.
    for (unsigned int i = 0; i != pages; i++) {
        buddy_allocatePage(potential_first + (0x1000 * i));
    }
    return (uintptr_t*)potential_first;
}
uintptr_t Kernel::PMM::buddy_getFirstPage() {
    uintptr_t i, j;

    // TODO(Lionel07): Optimize this by utilizing the buddy cache layers.
    uintptr_t *bitmap = buddy_startPage[0];

    for (i = 0; i < INDEX_FROM_BIT(kernel_totalPages); i++) {
        if (bitmap[i] != 0xFFFFFFFF) {
            for (j = 0; j < 32; j++) {
                uintptr_t testFrame = 0x1 << j;
                if (!(bitmap[i] & testFrame)) {
                    return i * 0x20 + j;
                }
            }
        }
    }
    return -1;
}

void Kernel::PMM::register_memoryRegion(memory_section_t * mem) {
    section_slots[kernel_usedSectionSlots] = mem;
    kernel_usedSectionSlots++;
    if(PMM_PRINTDEBUGSTATEMENTS) {
        printk(LOG_INFO,"pmm region: [%s] type %d, 0x%X -> 0x%X; ro %d; vmm mapped %d\n","new",mem->type,mem->start_address,mem->end_address,mem->read_only,mem->vmm_mapped);
    }
}