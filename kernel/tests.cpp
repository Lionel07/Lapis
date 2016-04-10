#include <mutex.h>
#include <log/printk.h>
void mutex_test()
{
    mutex_t test_a;
    mutex_t test_b;

    printk(LOG_DEBUG,"----------------------------------MUTEX TEST------------------------------------");

    if(mutex_acquire(&test_a))
    {
        
    }
    if(mutex_acquire(&test_b))
    {
        
    }

    if(mutex_acquire(&test_a))
    {
        printk(LOG_DEBUG,"test: %s!\n","Lock acquired when it shouldn't have!");
    }
    if(mutex_acquire(&test_b))
    {
        printk(LOG_DEBUG,"test: %s!\n","Lock acquired when it shouldn't have!");
    }

    printk(LOG_DEBUG,"%s!\n","--------------- 1/3 ---------------");

    mutex_release(&test_a);
    if(mutex_acquire(&test_b))
    {
        printk(LOG_DEBUG,"test: %s!\n","Lock acquired when it shouldn't have!");
    }

    if(mutex_acquire(&test_a))
    {
        
    }
    mutex_release(&test_b);

    if(mutex_acquire(&test_a))
    {
        printk(LOG_DEBUG,"test: %s!\n","Lock acquired when it shouldn't have!");
    }   
    if(mutex_acquire(&test_b))
    {
        
    }

    printk(LOG_DEBUG,"%s!\n","--------------- 2/3 ---------------");
    mutex_release(&test_a);
    mutex_release(&test_b);

    if(mutex_acquire(&test_a))
    {
        
    }
    if(mutex_acquire(&test_b))
    {
        
    }

    mutex_release(&test_a);
    mutex_release(&test_b);
    printk(LOG_DEBUG,"%s!\n","--------------- 3/3 ---------------");
}