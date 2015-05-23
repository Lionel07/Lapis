#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <arch/x86/idt.h>
#include <arch/x86/ports.h>
#include <arch/x86/irq.h>
#include <log/printk.h>

struct idt_entry arch_x86_idt[256];
struct idt_ptr   arch_x86_idtptr;

///Inits interrupt services
void ArchX86::IDT::Init()
{
	printk(LOG_INFO,"x86: Writing IDT and interrupt handlers to hardware... ");
	
	arch_x86_idtptr.limit = sizeof(struct idt_entry) * 256 -1;
   	arch_x86_idtptr.base  = (uint32_t)&arch_x86_idt;
	
	memset(&arch_x86_idt, 0, sizeof(struct idt_entry)*256);
	
	SetGate(0, (unsigned)isr0, 0x08, 0x8E);
	SetGate(1, (unsigned)isr1, 0x08, 0x8E);
	SetGate(2, (unsigned)isr2, 0x08, 0x8E);
	SetGate(3, (unsigned)isr3, 0x08, 0x8E);
	SetGate(4, (unsigned)isr4, 0x08, 0x8E);
	SetGate(5, (unsigned)isr5, 0x08, 0x8E);
	SetGate(6, (unsigned)isr6, 0x08, 0x8E);
	SetGate(7, (unsigned)isr7, 0x08, 0x8E);

	SetGate(8, (unsigned)isr8, 0x08, 0x8E);
	SetGate(9, (unsigned)isr9, 0x08, 0x8E);
	SetGate(10, (unsigned)isr10, 0x08, 0x8E);
	SetGate(11, (unsigned)isr11, 0x08, 0x8E);
	SetGate(12, (unsigned)isr12, 0x08, 0x8E);
	SetGate(13, (unsigned)isr13, 0x08, 0x8E);
	SetGate(14, (unsigned)isr14, 0x08, 0x8E);
	SetGate(15, (unsigned)isr15, 0x08, 0x8E);

	SetGate(16, (unsigned)isr16, 0x08, 0x8E);
	SetGate(17, (unsigned)isr17, 0x08, 0x8E);
	SetGate(18, (unsigned)isr18, 0x08, 0x8E);
	SetGate(19, (unsigned)isr19, 0x08, 0x8E);
	SetGate(20, (unsigned)isr20, 0x08, 0x8E);
	SetGate(21, (unsigned)isr21, 0x08, 0x8E);
	SetGate(22, (unsigned)isr22, 0x08, 0x8E);
	SetGate(23, (unsigned)isr23, 0x08, 0x8E);

	SetGate(24, (unsigned)isr24, 0x08, 0x8E);
	SetGate(25, (unsigned)isr25, 0x08, 0x8E);
	SetGate(26, (unsigned)isr26, 0x08, 0x8E);
	SetGate(27, (unsigned)isr27, 0x08, 0x8E);
	SetGate(28, (unsigned)isr28, 0x08, 0x8E);
	SetGate(29, (unsigned)isr29, 0x08, 0x8E);
	SetGate(30, (unsigned)isr30, 0x08, 0x8E);
	SetGate(31, (unsigned)isr31, 0x08, 0x8E);

	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
	
	SetGate(32, (unsigned)irq0, 0x08, 0x8E);
	SetGate(33, (unsigned)irq1, 0x08, 0x8E);
	SetGate(34, (unsigned)irq2, 0x08, 0x8E);
	SetGate(35, (unsigned)irq3, 0x08, 0x8E);
	SetGate(36, (unsigned)irq4, 0x08, 0x8E);
	SetGate(37, (unsigned)irq5, 0x08, 0x8E);
	SetGate(38, (unsigned)irq6, 0x08, 0x8E);
	SetGate(39, (unsigned)irq7, 0x08, 0x8E);
	SetGate(40, (unsigned)irq8, 0x08, 0x8E);
	SetGate(41, (unsigned)irq9, 0x08, 0x8E);
	SetGate(42, (unsigned)irq10, 0x08, 0x8E);
	SetGate(43, (unsigned)irq11, 0x08, 0x8E);
	SetGate(44, (unsigned)irq12, 0x08, 0x8E);
	SetGate(45, (unsigned)irq13, 0x08, 0x8E);
	SetGate(46, (unsigned)irq14, 0x08, 0x8E);
	SetGate(47, (unsigned)irq15, 0x08, 0x8E);
	
	idt_flush();
	
	printk(LOG_TAG,"Done");
}

///Sets a gate in the IDT
void ArchX86::IDT::SetGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    arch_x86_idt[num].base_lo = (base & 0xFFFF);
    arch_x86_idt[num].base_hi = (base >> 16) & 0xFFFF;

    arch_x86_idt[num].sel = sel;
    arch_x86_idt[num].always0 = 0;
    arch_x86_idt[num].flags = flags;
}
