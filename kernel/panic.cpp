#include <log/printk.h>

char * panicstr;

void panic (char *s) {
	panicstr = s;
	// Todo: Force sync
	printk(LOG_PANIC,"panic: %s\n", s);
}