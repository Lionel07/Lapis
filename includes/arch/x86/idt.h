#ifndef ARCH_X86_IDT_H
#define ARCH_X86_IDT_H
#include <stdint.h>
#include <stddef.h>

struct idt_entry
{
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

namespace ArchX86 {
    class IDT {
        static void SetGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
        public:
            static void Init();
    };
};

extern "C" void idt_flush();

#endif
