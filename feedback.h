#ifndef FEEDBACK_H_INCLUDED
#define FEEDBACK_H_INCLUDED

#include "configuration.h"
#include "passenger.h"

struct feedback
{
	struct passenger passengers[MAX_NUMBER_OF_PASSENGERS];
	int num_of_passengers;
	int values[MAX_NUMBER_OF_PASSENGERS];
};

#endif
