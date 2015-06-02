#include <log/printk.h>
#include <panic.h>

const char * panicstr;

void panic (const char *s) {
	panicstr = s;
	// Todo: Force sync
	printk(LOG_PANIC,"panic: %s\n", s);
	for(;;) {
#ifdef ARCH_x86
		asm("cli");
		asm("pause");
		asm("hlt");
#endif
	}
}