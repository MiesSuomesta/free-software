#include "ms_sleep.h"

#define sleepms cross_platform_sleep_ms

using namespace std;

/* Cross platform millisecond sleep */
void cross_platform_sleep_ms(unsigned long int time_to_sleep_in_ms)
{
   sleep_function_name ( sleep_time_multiplier_for_ms * time_to_sleep_in_ms );
}

int main()
{
	cout << "foo\n";
	sleepms (10000);
	cout << "faa\n";
	
	return 0;
}