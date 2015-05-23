#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <log/console.h>
#include <log/printk.h>
//#include <stdio.h>
//#include <string.h>

uint8_t text_console_fb[CONFIG_TEXTCONSOLE_FB_MX * CONFIG_TEXTCONSOLE_FB_MY * CONFIG_TEXTCONSOLE_FB_SCREENS * 2]; // Stores attribute byte as well on x86.
volatile uint8_t term_x;
volatile uint8_t term_y;
uint32_t scroll_y;

int term_fb_flag_modified = 0;

void TextConsole::Init() {
	term_x = 0;
	term_y = 0;
	scroll_y = 0;
	printk(LOG_INFO,"console: Initialising text console"); printk(LOG_TAG,"Done");
	
}

void TextConsole::Printc(char c) {
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
			FramebufferAddChar(c,term_x,term_y);
			term_x++;
			break;		
	}
	
	if (term_x >= CONFIG_TEXTCONSOLE_FB_MX) {
		term_x = 0;
		term_y++;
	}
	// Scroll the screen if needed.
	UpdateScroll();
	// Move the hardware cursor.
	//text_console_shim_setCursor(term_x, term_y);
}

void TextConsole::Print(const char *c) {
	int i = 0;
	while (c[i]) {
		Printc(c[i++]);
	}
	FramebufferFlush();
}

void TextConsole::UpdateFramebuffer() {
	FramebufferFlush();
}

void TextConsole::FramebufferAddChar(char c, uint8_t x, uint8_t y) {
	text_console_fb[((y * CONFIG_TEXTCONSOLE_FB_MX) + x) * 2] = c; // Multiply by 2 so you can add attribute byte.
	text_console_fb[(((y * CONFIG_TEXTCONSOLE_FB_MX) + x) * 2) + 1] = 0x0F;
	term_fb_flag_modified = 1;
}

void TextConsole::FramebufferAddCharAttrib(uint8_t c, uint8_t x, uint8_t y) {
	text_console_fb[(((y * CONFIG_TEXTCONSOLE_FB_MX) + x) * 2) + 1] = c;
	term_fb_flag_modified = 1;
}

void TextConsole::FramebufferFlush() {
	if(term_fb_flag_modified == 1) {
		TextConsole_Shim_Flush((uint8_t*)&text_console_fb,scroll_y);
		term_fb_flag_modified = 0;
	}
}

void TextConsole::UpdateScroll() {
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