#ifndef DEVICE_H
#define DEVICE_H
#include <driver.h>
#include <mutex.h>

#define DEVICE_INTERFACE_KERNEL   0x0
#define DEVICE_INTERFACE_PCI      0x1
#define DEVICE_INTERFACE_USB      0x2
#define DEVICE_INTERFACE_ACPI     0x3
#define DEVICE_INTERFACE_IO       0x4

#define DEVICE_TYPE_NAMESPACE     0x0
#define DEVICE_TYPE_BUS           0x1
#define DEVICE_TYPE_HARDWARE      0x2
#define DEVICE_TYPE_INTERGRATED   0x3

#define DEVICE_STATUS_UNSET       0x0
#define DEVICE_STATUS_HALTED      0x1
#define DEVICE_STATUS_ZOMBIE      0x2
#define DEVICE_STATUS_ONLINE      0x3
#define DEVICE_STATUS_PARTIALY    0x4
#define DEVICE_STATUS_ABORTED     0x5
typedef struct {
    uint32_t dev_class;
    uint32_t dev_type;
} kernel_device_config_t;

typedef struct {
    char *name;
    uint32_t id;
    uint32_t type;
    uint32_t flags;
    uint32_t permissions;
    uint32_t interface;
    uint32_t status;
    mutex_t * mutex;
    ///Driver
    driver_t * driver;
    ///PCI Configurationm
    //TODO
    ///USB Configuration
    //TODO
    ///ACPI Configuration
    //TODO
    //int (*recieve)(driver_msg_t*);
} device_t;

uint32_t device_start(device_t * dev);
uint32_t device_stop(device_t * dev);
#endif