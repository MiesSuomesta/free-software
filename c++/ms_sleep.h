 
#ifndef LJA_MS_SLEEP 
#define LJA_MS_SLEEP 1

#include <iostream>

/*
 Prepare defines for millisecond sleep function that is cross-platform
*/
#ifdef _WIN32
#  include <Windows.h>
#  define sleep_function_name           Sleep
#  define sleep_time_multiplier_for_ms      1
#else
#  include <unistd.h>
#  define sleep_function_name           usleep
#  define sleep_time_multiplier_for_ms      1000
#endif


void cross_platform_sleep_ms(unsigned long int time_to_sleep_in_ms);

#endif /* Main */
 
 