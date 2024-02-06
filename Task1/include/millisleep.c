#include <time.h>
#include "./millisleep.h"

static struct timespec timespec_from_ms(long ms)
{
        struct timespec timespec;
        timespec.tv_sec = ms / 1000;
        timespec.tv_nsec = (ms - timespec.tv_sec * 1000) * 1000000;
        return timespec;
}

void millisleep(long ms)
{
        struct timespec ts = timespec_from_ms(ms);
        if (nanosleep(&ts, NULL) != 0) {
                // This shouldn't be EINVAL and is unlikely to be EFAULT, so it
                // probably just means we got interrupted (which is also very
                // unlikely).
                PANIC("nanosleep() failed");
        }
}