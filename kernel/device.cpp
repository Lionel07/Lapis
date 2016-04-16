#include <driver.h>
#include <device.h>

#include <string.h>
#include <log/printk.h>

uint32_t device_stop(device_t * dev)
{
    if(dev == NULL) return 0xFFFFFFFF;
    printk(LOG_DEBUG,"device: Shutting down device %s... \n",dev->name);
    if(dev->status!=DEVICE_STATUS_ONLINE) //Device is already online, so do nothing to it
    {
        return dev->status;
    }
    else //Bring down device
    {
        if(dev->driver == NULL) return 0xFFFFFFFE;
        //mutex_lock(dev->mutex);
        int ret = dev->driver->stop();
        if(ret == 0)
        {
            dev->status = DEVICE_STATUS_HALTED;
        }
        else
        {
            dev->status = DEVICE_STATUS_ONLINE;
        }
        //mutex_unlock(dev->mutex);
    }
    return dev->status; 
}
/**
Starts a device. Returns an error code or the device status set
**/
uint32_t device_start(device_t * dev)
{
    if(dev == NULL) return 0xFFFFFFFF;
    
    if(dev->status==DEVICE_STATUS_ONLINE) //Device is already online, so do nothing to it
    {
        return DEVICE_STATUS_ONLINE;
    }
    printk(LOG_DEBUG,"device: Bringing up device %s... \n",dev->name);
    if(dev->driver == NULL)
    {
        return 0xFFFFFFFE;
    }
    //mutex_lock(dev->mutex);
    int ret = dev->driver->start();
    if(ret == 0)
    {
        dev->status = DEVICE_STATUS_ONLINE;
    }
    else
    {
        printk(LOG_PANIC,"device: Could not bring up device %s (returned 0x%X)!\n",dev->name,ret);
        dev->status = DEVICE_STATUS_ABORTED;
    }
    //mutex_unlock(dev->mutex);
    return dev->status; 
}