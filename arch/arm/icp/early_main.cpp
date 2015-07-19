#include <log/printk.h>
#include <log/console.h>
#include <version.h>

void kmain();
extern "C" void early_kernel_main() {
    TextConsole::Init();
    Kernel::Version::printKernelVersion();
    printk(LOG_NOTICE,"\n");
    printk(LOG_NOTICE,"========== Started Kernel Arch Init ==========\n");
    printk(LOG_NOTICE,"========== Finished Kernel Arch Init =========\n");
    kmain();
}