#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <log/console.h>
//#include <stdio.h>
//#include <string.h>

#define CONFIG_TEXTCONSOLE_FB_MX 80
#define CONFIG_TEXTCONSOLE_FB_MY 25
#define CONFIG_TEXTCONSOLE_FB_SCREENS 4

volatile uint8_t term_x = 0;
volatile uint8_t term_y = 0;
uint32_t scroll_y = 0;
int term_fb_flag_modified = 0;
uint8_t text_console_fb[CONFIG_TEXTCONSOLE_FB_MX * CONFIG_TEXTCONSOLE_FB_MY * CONFIG_TEXTCONSOLE_FB_SCREENS * 2]; // Stores attribute byte as well on x86.

void text_console_printc(char c) {
	switch(c) {
		case 0x08:
			if(term_x) {
				term_x--;
			}
			break;
		case 0x09:
			term_x = (term_x+8) & ~(8-1);
			break;
		case '\r':
			term_x = 0;
			break;
		case '\n':
			term_fb_flag_modified = 1;
			term_x = 0;
			term_y++;
			break;
		default:
			text_console_fbAddChar(c,term_x,term_y);
			term_x++;
			break;		
	}
	
	if (term_x >= CONFIG_TEXTCONSOLE_FB_MX) {
		term_x = 0;
		term_y++;
	}
	// Scroll the screen if needed.
	text_console_scroll();
	// Move the hardware cursor.
	//text_console_shim_setCursor(term_x, term_y);
}

void text_console_print(const char *c) {
	int i = 0;
	while (c[i]) {
		text_console_printc(c[i++]);
	}
	text_console_fb_flush();
}



void text_console_init() {

}

void text_console_fbAddChar(char c, uint8_t x, uint8_t y) {
	text_console_fb[((y * CONFIG_TEXTCONSOLE_FB_MX) + x) * 2] = c; // Multiply by 2 so you can add attribute byte.
	text_console_fb[(((y * CONFIG_TEXTCONSOLE_FB_MX) + x) * 2) + 1] = 0x0F;
	term_fb_flag_modified = 1;
}

void text_console_fb_flush() {
	if(term_fb_flag_modified == 1) {
		text_console_fb_shim_flush((uint8_t*)&text_console_fb,scroll_y);
		term_fb_flag_modified = 0;
	}
}

void text_console_scroll() {
	if(CONFIG_TEXTCONSOLE_FB_MY + scroll_y < term_y){
		scroll_y++;
	}
	// Resets if it gets to big
	// TODO: Replace this with a less destructive thingy
	if(scroll_y >= (CONFIG_TEXTCONSOLE_FB_MY - 2) * CONFIG_TEXTCONSOLE_FB_SCREENS) {
		uint32_t screensize = (CONFIG_TEXTCONSOLE_FB_MX * CONFIG_TEXTCONSOLE_FB_MY) * 2;
		memset(&text_console_fb,0,screensize * CONFIG_TEXTCONSOLE_FB_SCREENS);
		scroll_y = 0;
		term_y = 0;
	}
}