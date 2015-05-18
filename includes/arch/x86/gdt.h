#ifndef ARCH_X86_GDT_H
#define ARCH_X86_GDT_H
#include <stdint.h>
#include <stddef.h>


struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));


namespace ArchX86 {
    class GDT {
        static void SetGate(signed int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
        public:
            static void Init();
    };
};

extern "C" void gdt_flush(uint32_t gdt_ptr);
#endif