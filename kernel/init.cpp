#include <log/printk.h>
#include <pmm.h>
#include <vmm.h>
#include <panic.h>
void mutex_test();
int kmain(/*KernelInfo * k*/) {
	
	init_static_memregions();
	Kernel::PMM::init();
	Kernel::VMM::init();
	printk(LOG_INFO,"kernel: Finished Init!\n");
	//mutex_test();
	panic("try me");
	for(;;) {
		
	}
	return 0;
}