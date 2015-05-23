#include <log/console.h>
#include <stdarg.h>
#include <string.h>
#include <log/printk.h>

const signed int kernel_log_severityMask = -1; // -1 = nomask
const signed int kernel_log_defaultSeverity = LOG_INFO;
signed int kernel_log_lastMask;

extern volatile uint8_t term_x;
extern volatile uint8_t term_y;

char* itoh(int i, char *buf);
int itoa(int value, char *sp, int radix);

void printk(const signed int severity, const char *fmt, ...) {
	if(severity < kernel_log_severityMask) {
		if (severity == LOG_CONT || severity == LOG_TAG) {
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


	if (severity == LOG_TAG) {
		size_t len = strlen(fmt);
		uint8_t attribute = 0x0A;
		if ((strcmp(fmt, "Fail") == 0) || strcmp(fmt, "Fault") == 0) {
			attribute = 0x0C;
		}
		TextConsole::FramebufferAddChar('[',76-len,term_y);
		for(size_t i = 0; i < len; i++) {
			TextConsole::FramebufferAddChar(fmt[i],(78-len) + i,term_y);
			
			TextConsole::FramebufferAddCharAttrib(attribute ,(78-len) + i,term_y);
		}
		TextConsole::FramebufferAddChar(' ',78,term_y);
		TextConsole::FramebufferAddChar(']',79,term_y);
		term_y++;
		term_x = 0;
		return;
	}

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
			case 'd':
				i = va_arg(argp, int);
				itoa((int)i, fmtbuf, 10);
				TextConsole::Print((char*)fmtbuf);
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