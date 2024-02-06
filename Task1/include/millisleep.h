#pragma once

#include <time.h>
#include "./panic.h"

/**
 * I wonder what this function does... 
 * 
 * Note that unlike Java, this function panics on interrupt. 
 */
void millisleep(long ms);