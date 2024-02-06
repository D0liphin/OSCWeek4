#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "./panic.h"

__attribute__((noinline)) __attribute__((cold)) __attribute__((noreturn)) void
panic(int line_nr, char const *filename, char const *restrict format, ...)
{
#define STARS5 "*****"
#define STARS10 "**********"
#define STARS35 STARS10 STARS10 STARS10 STARS5
        va_list args;
        va_start(args, format);
        fprintf(stderr, "\e[0;31m" STARS35 "* panic! *" STARS35 "\e[0m\n");
        fprintf(stderr, "panicked at %s:%d\n", filename, line_nr);
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");
        va_end(args);
        exit(EXIT_FAILURE);
        __builtin_unreachable();
}

void todo(int line_nr, char const *filename)
{
        panic(line_nr, filename, "not yet implemented");
}