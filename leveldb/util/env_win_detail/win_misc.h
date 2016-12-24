#ifndef INCLUDE_GUARD_misc_1364388690
#define INCLUDE_GUARD_misc_1364388690

#include <string>

#include "winapi.h"

#define fread_unlocked _fread_nolock


bool ftruncate(const winapi::File& fd, int64_t length);

void usleep(int usec);
int getpagesize(void);

#endif

