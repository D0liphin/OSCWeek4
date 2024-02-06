#pragma once

__attribute__((noinline)) __attribute__((cold)) __attribute__((noreturn)) void
panic(int line_nr, char const *filename, char const *format, ...);

void todo(int line_nr, char const *filename);

/**
 * Panic and abort this program, with an error message first printed to 
 * `stderr`. You do not need to add a newline yourself. Pass the arguments as
 * you would to `printf()`.
 */
#define PANIC(...) panic(__LINE__, __FILE__, __VA_ARGS__)

/**
 * Mark an unreachable path as 'TODO'.
 */
#define TODO() todo(__LINE__, __FILE__)