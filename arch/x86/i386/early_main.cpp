#include <log/printk.h>
#include <version.h>
void arch_x86_gdt_init();
void log_printKernelVersion();

extern "C" void early_kernel_main() {
	Kernel::Version::printKernelVersion();
	
	printk(LOG_NOTICE,"========== Started Kernel Arch Init ==========\n");
	//Start to initialise the hardware
	arch_x86_gdt_init(); 

	printk(LOG_INFO,"Finished with x86 specific initialization - Moving onto core.\n");
	printk(LOG_NOTICE,"========== Finished Kernel Arch Init =========\n");
}		