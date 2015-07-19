#include <stdint.h>
#include <stddef.h>

#define SERIAL_BASE 0x16000000
#define SERIAL_FLAG_REGISTER 0x18
#define SERIAL_BUFFER_FULL (1 << 5)

void arm_integratorcp_driver_serial_putc (char c)
{
    /* Wait until the serial buffer is empty */
    while (*(volatile unsigned long*)(SERIAL_BASE + SERIAL_FLAG_REGISTER)
           & (SERIAL_BUFFER_FULL));
    /* Put our character, c, into the serial buffer */
    *(volatile unsigned long*)SERIAL_BASE = c;
    if(c == '\n')
    {
        arm_integratorcp_driver_serial_putc('\r');
    }
}

void TextConsole_Shim_WriteChar(uint8_t c,int x, int y) {
    x = x; y = y;
    arm_integratorcp_driver_serial_putc(c);
}