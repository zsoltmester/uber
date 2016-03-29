#ifndef ROUTE_H_INCLUDED
#define ROUTE_H_INCLUDED

#include "passenger.h"

struct route {
	char * destination;
	struct passenger * passengers;
	int num_of_passengers;
};

int add_route(char * destination, int max_number_of_routes, int * num_of_routes, struct route * routes);
int is_route_available(char * destination, int num_of_routes, struct route * routes);
int remove_route(char * destination, int * num_of_routes, struct route * routes);

int add_passenger(char * name, char * phone, time_t reg_time, struct route * route, int max_number_of_passengers);
int is_passenger_available(char * name, struct route * route);

char ** routes_to_lines(int num_of_routes, struct route * routes);
int parse_lines(char ** lines, int num_of_lines, int max_number_of_routes, int * num_of_routes, struct route * routes);

#endif
