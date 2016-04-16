#ifndef DRIVER_SERIAL_H
#define DRIVER_SERIAL_H

#define DRIVER_SERIAL_COM1 0x3f8

int driver_serial_start();
void driver_serial_init();
void driver_serial_putc(char c);

#endif