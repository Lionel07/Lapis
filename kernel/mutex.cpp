/*
    mutex.cpp

    mutex_t * mutex_name; is a mutex (binary semophore)

    Developer notes: PLEASE MAKE SURE THAT THESE ARE AS ATOMIC AS POSSIBLE.
    Yes, this means writing these in ASM for the _atomic versions

    Operations:
    * Lock: bool mutex_acquire(mutex_t * mutex);
    * Unlock: void mutex_release(mutex_t * mutex);
    * Spin until acquired: void mutex_acquire_spin(mutex_t * mutex);

    Each one of these has an atomic version, prefixed by _atomic.
*/
#include <stdint.h>
#include <stddef.h>
#include <mutex.h>
#include <log/printk.h>

//#define MUTEX_ALWAYS_ATOMIC
#define MUTEX_DEBUG

bool mutex_acquire(mutex_t * mutex)
{
    #ifdef MUTEX_ALWAYS_ATOMIC
    return mutex_acquire_atomic(mutex);
    #else
    if(*mutex == 0)
    {
        *mutex = 1;
        #ifdef MUTEX_DEBUG
        printk(LOG_DEBUG,"mutex: SUCCEEDED to acquire 0x%x\n",mutex);
        #endif
        return true;
    }
    #ifdef MUTEX_DEBUG
    printk(LOG_DEBUG,"mutex: FAILED to acquire 0x%x\n",mutex);
    #endif
    return false;
    #endif
}   

bool mutex_acquire_atomic(mutex_t * mutex)
{
    unsigned char res = 1;
    #ifdef ARCHx86
    asm volatile (
        "mov %%eax, 1;"
        "lock; cmpxchg %1, (%2)"                             
        : "=r"(res)         
        : "r"(res), "r"(mutex)
        : "%1","memory");
    #endif

    #ifdef MUTEX_DEBUG
    if(res)
    {
        printk(LOG_DEBUG,"mutex: SUCCEEDED to acquire 0x%x\n",mutex);
    }
    else
    {
        printk(LOG_DEBUG,"mutex: FAILED to acquire 0x%x\n",mutex);
    }
    #endif
    return (bool)res;
}

void mutex_release(mutex_t * mutex)
{
    *mutex = 0;
    #ifdef MUTEX_DEBUG
    printk(LOG_DEBUG,"mutex: RELEASED 0x%x\n",mutex);
    #endif
}

void mutex_acquire_spin(mutex_t * mutex)
{
    while(true)
    {
        #ifndef MUTEX_ALWAYS_ATOMIC
        if(mutex_acquire(mutex) == true)
        {
            return;
        }
        #else
        if(mutex_acquire(mutex) == true)
        {
            return;
        }
        #endif
    }
}