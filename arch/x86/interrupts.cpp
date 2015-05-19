#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <arch/x86/idt.h>
#include <arch/x86/ports.h>
#include <arch/x86/irq.h>
#include <log/printk.h>

const char *exception_messages[] =
{
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",

	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",

	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

interrupt_handler_t interrupt_handlers [0xff];

extern "C" void fault_handler(struct regs *r)
{
	if (interrupt_handlers[r->int_no] != 0)
	{
		interrupt_handlers[r->int_no] (r);
		return;
	} else {
			//																|
			printk(LOG_CRIT,"========= Started Kernel Fault Handler ======="); printk(LOG_TAG,"Fault");
			printk(LOG_CRIT,"type:%d (error %d),(%s)\n",r->int_no,r->err_code,exception_messages[r->int_no]);
			printk(LOG_CRIT,"gs:  0x%x fs:0x%x es:0x%x\nds:  0x%x cs:0x%x ss:0x%x\n",r->gs,r->fs,r->es,r->ds,r->cs,r->ss);
			printk(LOG_CRIT,"esp: 0x%x ebp: 0x%x uesp: 0x%x\n",r->useless_value,r->ebp,r->useresp);
			printk(LOG_CRIT,"eax: 0x%x ebx: 0x%x ecx: 0x%x edx: 0x%x\n",r->eax,r->ebx,r->ecx,r->edx);
			printk(LOG_CRIT,"esi: 0x%x edi: 0x%x eip: 0x%x eflags: 0x%x \n",r->esi,r->edi,r->eip,r->eflags);
			printk(LOG_CRIT,"========== Ended Kernel Fault Handler ========\n");
			return;
	}
	if(r->int_no < 32)
	{
		printk(LOG_CRIT,"========= Started Kernel Fault Handler ======="); printk(LOG_TAG,"Fault");
		printk(LOG_CRIT,"type:%d (error %d),(%s)\n",r->int_no,r->err_code,exception_messages[r->int_no]);
		printk(LOG_CRIT,"gs:  0x%x fs:0x%x es:0x%x\nds:  0x%x cs:0x%x ss:0x%x\n",r->gs,r->fs,r->es,r->ds,r->cs,r->ss);
		printk(LOG_CRIT,"esp: 0x%x ebp:0x%x uesp:0x%x\n",r->useless_value,r->ebp,r->useresp);
		printk(LOG_CRIT,"eax: 0x%x ebx:0x%x ecx:0x%x edx:0x%x\n",r->eax,r->ebx,r->ecx,r->edx);
		printk(LOG_CRIT,"esi: 0x%x edi:0x%x eip:0x%x eflags:0x%x\n",r->esi,r->edi,r->eip,r->eflags);
		printk(LOG_CRIT,"========== Ended Kernel Fault Handler ========\n");
	}
}

void register_interrupt_handler (uint8_t n, interrupt_handler_t h)
{
	interrupt_handlers [n] = h;
	
}

/** This clears the handler for a given IRQ **/
void deregister_interrupt_handler (uint8_t n)
{
	interrupt_handlers [n] = 0;
}

extern "C" void irq_handler(struct regs *r)
{
	/* This is a blank function pointer */
	if (interrupt_handlers[r->int_no] != 0){
		interrupt_handlers[r->int_no] (r);
	}
	else {
		printk(LOG_WARN,"irq: Recieved unhandled interrupt %d (IRQ%d)\n",r->int_no,r->int_no-32);
	}
	/* If the IDT entry that was invoked was greater than 40
	*  (meaning IRQ8 - 15), then we need to send an EOI to
	*  the slave controller */
	if (r->int_no >= 40) {
		outb(0xA0, 0x20);
	}

	/* In either case, we need to send an EOI to the master
	*  interrupt controller too */
	outb(0x20, 0x20);
}
