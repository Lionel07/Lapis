#include <log/printk.h>
#include <log/console.h>
#include <version.h>

#include <arch/x86/gdt.h>
#include <arch/x86/idt.h>
void kmain();
extern "C" void early_kernel_main() {
	TextConsole::Init();
	Kernel::Version::printKernelVersion();
	printk(LOG_NOTICE,"\n");
	printk(LOG_NOTICE,"========== Started Kernel Arch Init ==========\n");
	//Start to initialise the hardware
	ArchX86::GDT::Init(); 
	ArchX86::IDT::Init();
	printk(LOG_NOTICE,"========== Finished Kernel Arch Init =========\n");
	//kmain();
}		