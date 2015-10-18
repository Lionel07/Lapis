#include <log/printk.h>
#include <pmm.h>
#include <vmm.h>
#include <panic.h>
int kmain(/*KernelInfo * k*/) {

	init_static_memregions();
	Kernel::PMM::init();
	Kernel::VMM::init();
	printk(LOG_INFO,"kernel: Finished Init!\n");
	for(;;) {
		
	}
	return 0;
}