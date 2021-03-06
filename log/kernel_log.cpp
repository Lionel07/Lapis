#include <config.h>
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
void driver_serial_putc(char c);

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
		#ifdef USING_LOG_FANCYTAGS
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

		#ifdef ARCHx86
			driver_serial_putc(0x1B);
			driver_serial_putc('[');
		    driver_serial_putc('3');
		    driver_serial_putc('0'+attribute % 8);
		    driver_serial_putc('m');
			
			driver_serial_putc(' ');
			driver_serial_putc('[');
	    	for(size_t i = 0; i < len; i++) {
				driver_serial_putc(fmt[i]);
			}
			driver_serial_putc(']');

			driver_serial_putc('\n');
		#endif

		#else
		printk(kernel_log_lastMask," [");
		printk(kernel_log_lastMask,fmt);
		printk(kernel_log_lastMask,"]\n");
		term_y++;
		term_x = 0;
		#endif
		return;
	}

	uint8_t attribute = CONSOLE_DEFAULT_COLOR;

 	if(severity == LOG_ERR || severity ==  LOG_CRIT
 	   || severity ==  LOG_ALERT || severity ==  LOG_PANIC)
 	{
 		attribute = 0x04;
 	}
 	if(severity == LOG_DEBUG)
 	{
 		attribute = 0x07;
 	}
 	if(severity == LOG_INFO)
 	{
 		attribute = 0x0F;
 	}
 	if(severity == LOG_NOTICE)
 	{
 		attribute = 0x0B;
 	}
 	if(severity == LOG_WARN)
 	{
 		attribute = 0x0E;
 	}
	va_start(argp, fmt);
	for(p = fmt; *p != '\0'; p++) {
		if(*p != '%') {
			TextConsole::Printc(*p,attribute);
			continue;
		}
		switch(*++p) {
			case 'c':
				i = va_arg(argp, int);
				TextConsole::Printc(i,attribute);
				break;
			case 's':
				s = va_arg(argp, char *);
				TextConsole::PrintAttribute((char*)s, CONSOLE_STRING_COLOR);
				break;
			case 'x':
			case 'X':
				i = va_arg(argp, int);
				s = itoh(i, fmtbuf);
				TextConsole::PrintAttribute((char*)s,0x0E);
				break;
			case 'd':
				i = va_arg(argp, int);
				itoa((int)i, fmtbuf, 10);
				TextConsole::PrintAttribute((char*)fmtbuf, 0x03);
				break;
			case '%':
				TextConsole::Printc('%',attribute);
				break;
		}
	}
	va_end(argp);
	//TODO: Remove this later, after we got an interrupt system working.
	TextConsole::UpdateFramebuffer();
}