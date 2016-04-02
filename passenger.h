#ifndef PASSENGER_H_INCLUDED
#define PASSENGER_H_INCLUDED

#include <time.h>

struct passenger
{
	char name[16];
	char phone[16];
	time_t mod_time;
};

#endif
