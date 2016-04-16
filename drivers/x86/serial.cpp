#include <arch/x86/ports.h>
#include <drivers/serial.h>
#include <driver.h>
#include <device.h>

driver_t    serial_driver;
device_t    serial_device;

bool driver_serial_setup = false;

int driver_serial_start() {
    // Taken from OSDev.org
    outb(DRIVER_SERIAL_COM1 + 1, 0x00);    // Disable all interrupts
    outb(DRIVER_SERIAL_COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(DRIVER_SERIAL_COM1 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(DRIVER_SERIAL_COM1 + 1, 0x00);    //                  (hi byte)
    outb(DRIVER_SERIAL_COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(DRIVER_SERIAL_COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(DRIVER_SERIAL_COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    driver_serial_setup = true;

    // Send terminal commands to clear screen.
    #ifdef SERIAL_CLEAR
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
    #endif
    return 0;
}

void driver_serial_putc(char c)
{
    if(!driver_serial_setup)
    {
        //driver_serial_start();
        return;
    }
    while ((inb(DRIVER_SERIAL_COM1 + 5) & 0x20) == 0);
    outb(DRIVER_SERIAL_COM1,c);
}

void driver_serial_init()
{
    serial_driver.start = &driver_serial_start;
    serial_device.name       = "com1";
    serial_device.type       = DEVICE_TYPE_HARDWARE;
    serial_device.flags      = 0;
    serial_device.interface  = DEVICE_INTERFACE_IO;
    serial_device.driver     = &serial_driver;
    device_start(&serial_device);
}
