/**
 * A C implementation of the Java solution to the 'dining philosopher problem' 
 * provided in week 4 of 5CCS2OSC. 
 * 
 * This code exhibits a variety of bad practices in its attempt to prioritize a
 * closer mapping to the original (not very good) Java implementation. 
 */

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>
// For whatever reason, my IDE doesn't pick up nanosleep(), so uncomment this if
// you need.
// int nanosleep(const struct timespec *req, struct timespec *_Nullable rem);
#include <time.h>

__attribute__((noinline)) __attribute__((cold)) __attribute__((noreturn)) void
panic(int line_nr, char const *filename, char const *restrict format, ...)
{
#define STARS5 "*****"
#define STARS10 STARS5 STARS5
#define STARS35 STARS10 STARS10 STARS10 STARS5
        va_list args;
        va_start(args, format);
        fprintf(stderr, "\e[0;31m" STARS35 "* panic! *" STARS35 "\e[0m\n");
        fprintf(stderr, "panicked at %s:%d\n", filename, line_nr);
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");
        va_end(args);
        exit(EXIT_FAILURE);
}

#define PANIC(...) panic(__LINE__, __FILE__, __VA_ARGS__)

struct timespec timespec_from_ms(long ms)
{
        struct timespec timespec;
        timespec.tv_sec = ms / 1000;
        timespec.tv_nsec = (ms - timespec.tv_sec * 1000) * 1000000;
        return timespec;
}

/**
 * I wonder what this function does... 
 * 
 * Note that unlike Java, this function panics on interrupt. 
 */
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

/**
 * Literally just a mutex wrapper. Note that in Java we are using 
 * `ReentrantLock` which is not quite the same as `PTHREAD_MUTEX_DEFAULT`, which
 * is what we're using here. I believe that Java's `ReentrantLock` is much more 
 * similar to a mutex of type `PTHREAD_MUTEX_RECURSIVE`.
 */
struct fork {
        pthread_mutex_t mtx;
};

/**
 * A custom typedef just for readability. Lots of style guides say don't do 
 * this... lots of style guides say do do this. Up to you. I prefer it.
 */
typedef void *(*pthread_routine_t)(void *);

/**
 * Construct a new `struct fork` with a valid mutex.
 */
struct fork fork_new()
{
        struct fork this;
        if (pthread_mutex_init(&this.mtx, NULL) != 0) {
                // This is pretty much never going to happen. It's sort of like
                // checking if malloc() fails... It feels like you ought to do 
                // it in C, since it's UB otherwise but for little toy projects 
                // you probably would be fine to let unsound code like this leak
                // through.
                PANIC("pthread_mutex_init() failed");
        }
        return this;
}

bool fork_pick_up(struct fork *restrict fork)
{
        int e = pthread_mutex_trylock(&fork->mtx);
        switch (e) {
        case 0:
                return true;
        // As per the docs: The mutex could not be acquired because it was 
        // already locked.
        case EBUSY:
                return false;
        default:
                // EINVAL and EAGAIN cannot happen assuming the the fork is
                // being used correctly.
                PANIC("invalid use of `struct fork`");
        }
}

void fork_put_down(struct fork *restrict fork)
{
        if (pthread_mutex_unlock(&fork->mtx) != 0) {
                PANIC("pthread_mutex_unlock() failed");
        }
}

struct philosopher {
        size_t id;
        struct fork *fork1;
        struct fork *fork2;
        uint32_t thinking_count;
        uint32_t eating_count;
};

void philosopher_announce(struct philosopher const *this, char const *msg)
{
        printf("Philosopher %zu %s\n", this->id, msg);
}

/**
 * Get a random `int` in the range `min..=max`.
 */
int randrange(int min, int max)
{
        return rand() % (max + 1 - min) + min;
}

void philosopher_act(struct philosopher const *this, char const *msg)
{
        philosopher_announce(this, msg);
        millisleep(randrange(500, 3500));
}

/**
 * This is equivalent to the `run()` method when we make something `Runnable`.
 * Note that this also has a return value, but we return by assigning to some 
 * memory location. You can think about the caching implications of all this.
 * Mutexes need to be shared though, we can't really get over that.
 */
void *philosopher_routine(struct philosopher *this)
{
        while (true) {
                philosopher_act(this, "is thinking...");
                this->thinking_count++;
                if (fork_pick_up(this->fork1)) {
                        philosopher_announce(this, "has picked up left fork.");
                } else {
                        continue;
                }

                // I add this here just to make the whole deadlock thing a bit
                // more apparent. If we don't have it, it's not as interesting.
                philosopher_act(this, "is picking up right fork...");

                if (fork_pick_up(this->fork2)) {
                        philosopher_announce(this, "has picked up right fork.");
                        philosopher_act(this, "is eating");
                        this->eating_count++;
                        fork_put_down(this->fork1);
                        fork_put_down(this->fork2);
                } else {
                        philosopher_announce(this, "has put down left fork.");
                        fork_put_down(this->fork1);
                }
        }
        return NULL;
}

/**
 * This is like doing 
 * 
 * ```java
 * var thread = new Thread(philosopher);
 * thread.start();
 * return thread;
 * ```
 */
pthread_t philosopher_start(struct philosopher *restrict this)
{
        pthread_t thread;
        int e = pthread_create(&thread, NULL, (pthread_routine_t)philosopher_routine, this);
        if (e != 0) {
                PANIC("bad pthread_create()");
        }
        return thread;
}

#define SIZE 5

int main()
{
        srand(time(NULL));

        struct fork forks[SIZE];
        for (size_t i = 0; i < SIZE; ++i) {
                forks[i] = fork_new();
        }

        struct philosopher philosophers[SIZE];
        for (size_t i = 0; i < SIZE; ++i) {
                philosophers[i] = (struct philosopher){
                        .id = i + 1,
                        .fork1 = &forks[i],
                        .fork2 = &forks[(i + 1) % SIZE],
                        .eating_count = 0,
                        .thinking_count = 0,
                };
        }

        pthread_t threads[5];
        for (size_t i = 0; i < SIZE; ++i) {
                threads[i] = philosopher_start(&philosophers[i]);
        }

        // Not present in example, but we actually need to join on these threads
        // so that they run.
        for (size_t i = 0; i < SIZE; ++i) {
                pthread_join(threads[i], NULL);
        }

        return 0;
}
