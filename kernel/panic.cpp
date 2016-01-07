#include <log/printk.h>
#include <panic.h>
void halt_and_catch_fire();
const char * panic_string;

void panic (const char *s) {
	panic_string = s;
	// Todo: Force sync
	printk(LOG_PANIC,"panic: %s\n", s);
	halt_and_catch_fire();
}