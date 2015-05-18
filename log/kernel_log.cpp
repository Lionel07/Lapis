#include <log/console.h>
#include <stdarg.h>
#include <log/printk.h>

const signed int kernel_log_severityMask = -1; // -1 = nomask
const signed int kernel_log_defaultSeverity = LOG_INFO;
signed int kernel_log_lastMask;

char* itoh(int i, char *buf);

void printk(const signed int severity, const char *fmt, ...) {
	if(severity < kernel_log_severityMask) {
		if (severity == LOG_CONT) {
			if(kernel_log_lastMask < kernel_log_severityMask) {
				return;
			}
		} else {
			kernel_log_lastMask = severity;
			return;
		}
	}
	kernel_log_lastMask = severity;
	
	const char 	*p;
	va_list 	argp;
	int 		i;
	char 		*s;
	char 		fmtbuf[256];

	va_start(argp, fmt);

	for(p = fmt; *p != '\0'; p++) {
		if(*p != '%') {
			TextConsole::Printc(*p);
			continue;
		}
		switch(*++p) {
			case 'c':
				i = va_arg(argp, int);
				TextConsole::Printc(i);
				break;
			case 's':
				s = va_arg(argp, char *);
				TextConsole::Print((char*)s);
				break;
			case 'x':
			case 'X':
				i = va_arg(argp, int);
				s = itoh(i, fmtbuf);
				TextConsole::Print((char*)s);
				break;
			case '%':
				TextConsole::Printc('%');
				break;
		}
	}
	va_end(argp);
	//TODO: Remove this later, after we got an interrupt system working.
	TextConsole::UpdateFramebuffer();
}