#include <log/printk.h>
#include <pmm.h>
#include <panic.h>
int kmain(/*KernelInfo * k*/) {
	printk(LOG_INFO,"Control transfered to kernel init process.\n");
	printk(LOG_NOTICE,"========== Started Kernel Init ===============\n");
	Kernel::PMM::init();
	printk(LOG_WARN,"git: You're using the MASTER Branch, which is probably not what you intended.\n");
	printk(LOG_WARN,"git: All the interesting stuff is happening in other branches! Try one out!\n");
	printk(LOG_WARN,"Kernel init process ended - Development has not made it this far yet.\n");
	panic("No init called!");
	printk(LOG_NOTICE,"========== Finished Kernel Init ==============\n");
	for(;;) {
		
	}
	return 0;
}