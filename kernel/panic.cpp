#include <log/printk.h>
#include <panic.h>
void halt_and_catch_fire();
const char * panicstr;

void panic (const char *s) {
	panicstr = s;
	// Todo: Force sync
	printk(LOG_PANIC,"panic: %s\n", s);
	halt_and_catch_fire();
}