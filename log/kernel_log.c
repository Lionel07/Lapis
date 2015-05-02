#include <log/console.h>
#include <stdarg.h>
#include <log/printk.h>

const char *itoh_map = "0123456789ABCDEF";

const signed int kernel_log_severityMask = -1; // -1 = nomask
const signed int kernel_log_defaultSeverity = LOG_INFO;
signed int kernel_log_lastMask;
int getBits();
char* itoh(int i, char *buf) {
	int		n;
	int		b;
	int		z;
	int		s;
	
	s = getBits() / 4;
	
	for (z = 0, n = (s - 1); n > -1; --n)
	{
		b = (i >> (n * 4)) & 0xf;
		buf[z] = itoh_map[b];
		++z;
	}
	buf[z] = 0;
	return buf;
}

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
			text_console_printc(*p);
			continue;
		}
		switch(*++p) {
			case 'c':
				i = va_arg(argp, int);
				text_console_printc(i);
				break;
			case 's':
				s = va_arg(argp, char *);
				text_console_print((char*)s);
				break;
			case 'x':
			case 'X':
				i = va_arg(argp, int);
				s = itoh(i, fmtbuf);
				text_console_print((char*)s);
				break;
			case '%':
				text_console_printc('%');
				break;
		}
	}
	va_end(argp);
	//TODO: Remove this later, after we got an interrupt system working.
	text_console_fb_flush();
}