#include <log/printk.h>
#include <pmm.h>
#include <vmm.h>
#include <panic.h>
int kmain(/*KernelInfo * k*/) {

	init_static_memregions();
	Kernel::PMM::init();
	Kernel::VMM::init();
	vmm_page_t test = Kernel::VMM::addressToVmmPage(0x00100000);
	Kernel::VMM::printVmmPage(test);
	printk(LOG_INFO,"kernel: Finished Init!\n");
	for(;;) {
		
	}
	return 0;
}