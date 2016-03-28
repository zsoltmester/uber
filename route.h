#ifndef ROUTE_H_INCLUDED
#define ROUTE_H_INCLUDED

struct route {
	char * destination; 
};

int add_route(char * destination, int max_number_of_routes, int * num_of_routes, struct route * routes);
int is_route_available(char * destination, int num_of_routes, struct route * routes);

#endif
