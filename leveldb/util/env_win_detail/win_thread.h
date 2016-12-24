#ifndef INCLUDE_GUARD_thread_1364388468
#define INCLUDE_GUARD_thread_1364388468

#include <stdint.h>

#include "winapi.h"

typedef uint32_t winthread_t;

winthread_t winthread_self();

bool winthread_create(winthread_t *thread, void *(*start_routine) (void *), void *arg);

#endif
