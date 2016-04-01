#ifndef ROUTE_H_INCLUDED
#define ROUTE_H_INCLUDED

struct route {
	char destination[16];
	//struct passenger * passengers;
	int num_of_passengers;
};

int add_route(char destination[16], int max_number_of_routes, int *num_of_routes, struct route routes[]);
int is_route_available(char destination[16], int num_of_routes, struct route routes[]);
int remove_route(char destination[16], int *num_of_routes, struct route routes[]);

//int add_passenger(char * name, char * phone, time_t reg_time, struct route * route, int max_number_of_passengers);
//int is_passenger_available(char * name, struct route * route);

#endif
