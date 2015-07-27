#include <log/printk.h>
#include <pmm.h>
#include <panic.h>
int kmain(/*KernelInfo * k*/) {
	printk(LOG_INFO,"Control transfered to kernel init process.\n");
	printk(LOG_NOTICE,"========== Started Kernel Init ===============\n");
	init_static_memregions();
	Kernel::PMM::init();
	Kernel::PMM::debugPrintStatistics();
	printk(LOG_WARN,"Kernel init process ended - Development has not made it this far yet.\n");
	panic("No init called!");
	printk(LOG_NOTICE,"========== Finished Kernel Init ==============\n");
	for(;;) {
		
	}
	return 0;
}