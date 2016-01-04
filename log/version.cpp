#include <version.h>
#include <config.h>
#include <log/printk.h>

void Kernel::Version::printKernelVersion() {
	printk(LOG_NOTICE,"\nThe Lapis Kernel - 0.0.0.1 - Copyright Corwin McKnight 2016\n");
	printk(LOG_NOTICE,"Built for %s on %s\n",BOARD_NAME,__DATE__);
    #ifndef NO_VERSION_WARNING
    printk(LOG_NOTICE,"This software is in the early stages of development - Be wary when using it.\n");
    #endif
}