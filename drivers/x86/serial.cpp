#include <arch/x86/ports.h>
#define COM1 0x3f8
bool driver_serial_setup = false;
void driver_serial_putc(char c);
void driver_serial_init() {
    // Taken from OSDev.org
    outb(COM1 + 1, 0x00);    // Disable all interrupts
    outb(COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(COM1 + 1, 0x00);    //                  (hi byte)
    outb(COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    driver_serial_setup = true;

    // Send terminal commands to clear screen.
    driver_serial_putc(0x1B);
    driver_serial_putc('[');
    driver_serial_putc('0');
    driver_serial_putc('2');
    driver_serial_putc('J');

    driver_serial_putc(0x1B);
    driver_serial_putc('[');
    driver_serial_putc('1');
    driver_serial_putc(';');
    driver_serial_putc('1');
    driver_serial_putc('H');
}

void driver_serial_putc(char c)
{
    if(!driver_serial_setup)
    {
        driver_serial_init();

    }
    while ((inb(COM1 + 5) & 0x20) == 0);
    outb(COM1,c);
}

