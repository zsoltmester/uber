#ifndef ROUTE_H_INCLUDED
#define ROUTE_H_INCLUDED

#include "passenger.h"

struct route {
	char destination[16];
	int num_of_passengers;
	struct passenger passengers[64];
};

int add_route(char destination[16], int max_number_of_routes, int *num_of_routes, struct route routes[]);
int is_route_available(char destination[16], int num_of_routes, struct route routes[]);
int remove_route(char destination[16], int *num_of_routes, struct route routes[]);

int add_passenger(char name[16], char phone[16], int max_number_of_passengers, struct route * route);
int is_passenger_available(char name[16], struct route route);
int remove_passenger(char destination[16], char name[16], int num_of_routes, struct route routes[]);

#endif
