#ifndef TEXT_CONSOLE_H
#define TEXT_CONSOLE_H
#include <stdint.h>
#include <stddef.h>

#define CONFIG_TEXTCONSOLE_FB_MX 80
#define CONFIG_TEXTCONSOLE_FB_MY 25
#define CONFIG_TEXTCONSOLE_FB_SCREENS 4

class TextConsole {
	public:
		static void Printc(char c);
		static void Print(const char *c);
		static void Init(); // WARN: Don't use global constructor. We need to have control
		static void SetCursor(uint8_t x, uint8_t y);
		static void UpdateScroll();
		static void UpdateFramebuffer();
		static void SetColor(uint8_t color);
		static void ResetColor(uint8_t color);
	private:
		static void FramebufferAddChar(char c, uint8_t x, uint8_t y);
		static void FramebufferFlush();
};

void TextConsole_Shim_Printc(char c, uint8_t x, uint8_t y);
void TextConsole_Shim_SetCursor(uint8_t x,uint8_t y);
void TextConsole_Shim_Flush(uint8_t *flush,uint32_t scroll_y);

#endif