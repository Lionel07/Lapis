#ifndef MEMORY_H
#define MEMORY_H
#include <stdint.h>
typedef enum memory_section {
    INVALID,
    RESERVED,
    MMIO,
    FREE,
    KERNEL_HEAP,
    KERNEL_BOOKKEEPING,
    KERNEL_CODE,
} memory_section_type_t;

typedef struct {
    memory_section_type_t type;
    uintptr_t start_address;
    uintptr_t end_address;
    bool read_only;
    uintptr_t vmm_mapped;
    // TODO: Fill mapped address
} memory_section_t;

void init_static_memregions();

#endif