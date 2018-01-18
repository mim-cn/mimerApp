#ifndef TIMEUTIL_H
#define TIMEUTIL_H

#ifdef __APPLE__
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#ifdef OS_MSWIN
#  include <windows.h>
#endif
#ifdef OS_LINUX
#  include <unistd.h>
#endif
#include <time.h>
#include <stdlib.h>
#include "memUtil.h"

time_t _time(void);
char*  _timestr(size_t size);

#define uuid _time
#define uids _timestr

#endif // TIMEUTIL_H
