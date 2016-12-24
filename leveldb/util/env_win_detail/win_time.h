#ifndef INCLUDE_GUARD_time_1364388461
#define INCLUDE_GUARD_time_1364388461

#include "winapi.h"

int gettimeofday(struct timeval *__p, struct timezone  *__tz);
struct tm *localtime_r(const time_t *timep, struct tm *result);

#endif
