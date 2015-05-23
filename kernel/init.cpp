#include <log/printk.h>
#include <pmm.h>
int kmain(/*KernelInfo * k*/) {
	printk(LOG_INFO,"Control transfered to kernel init process.\n");
	printk(LOG_NOTICE,"========== Started Kernel Init ===============\n");
	Kernel::PMM::init();
	printk(LOG_WARN,"Kernel init process ended - Development has not made it this far yet.\n");
	for(;;) {
			
	}
	printk(LOG_NOTICE,"========== Finished Kernel Init ==============\n");
	for(;;) {
		
	}
	return 0;
}