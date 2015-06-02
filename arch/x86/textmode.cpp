#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <arch/x86/ports.h>

#define X86_CONSOLE_WIDTH  80
#define X86_CONSOLE_HEIGHT 25

#define X86_CONSOLE_COLOR 0x0F

volatile uint8_t *textmode_buffer=(uint8_t*)0xB8000;
//volatile uint16_t *textmode_buffer_16=(uint16_t*)0xB8000;
volatile uint8_t textmode_color = 0x0F;

void text_console_change_color(uint8_t color) {
	textmode_color = color;
}

void text_console_reset_color() {
	textmode_color = X86_CONSOLE_COLOR;
}

void text_console_setcursor(uint8_t x,uint8_t y)
{
	uint16_t location = y * X86_CONSOLE_WIDTH + x;
	outb(0x3D4, 14);
	outb(0x3D5, location >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, location);
}
/*
void text_console_scroll(int from,int to) { //TODO: Either depreciate, or convert to framebuffer.
	uint16_t blank = 0x20  | (textmode_color << 8);
	int i;
	for (i = from*80; i < to*80; i++) {
		textmode_buffer_16[i] = textmode_buffer_16[i+80];
	}

	for (i = to*80; i < (to+1)*80; i++)
	{
		textmode_buffer_16[i] = blank;
	}
}
*/
void TextConsole_Shim_Flush(uint8_t *flush,uint32_t scroll_y) {
	memcpy((uint8_t*)textmode_buffer,(uint8_t*)flush+((X86_CONSOLE_WIDTH*2)*scroll_y),X86_CONSOLE_WIDTH*X86_CONSOLE_HEIGHT*2);
}

uint8_t text_console_fb_shim_x86_addattribute() {
	return textmode_color;
}