#ifndef LAPIS_MUTEX_H
#define LAPIS_MUTEX_H
#include <stdint.h>
#include <stddef.h>

typedef uint8_t mutex_t;

bool mutex_acquire(mutex_t * mutex);
bool mutex_acquire_atomic(mutex_t * mutex);
void mutex_release(mutex_t * mutex);
void mutex_acquire_spin(mutex_t * mutex);

#endif