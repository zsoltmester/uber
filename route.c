#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "route.h"

int add_route(char * destination, int max_number_of_routes, int * num_of_routes, struct route * routes)
{
	if (*num_of_routes == max_number_of_routes) 
	{
		printf("[ERROR] No more routes allowed.\n");
		return 1;
	}
	if (strlen(destination) == 0) 
	{
		printf("[ERROR] Destination is empty.\n");
		return 1;
	}
	if (is_route_available(destination, *num_of_routes, routes)) 
	{
		printf("[ERROR] This destination is already available.\n");
		return 1;
	}
	
	routes[*num_of_routes].destination = malloc(strlen(destination));
	strcpy(routes[*num_of_routes].destination, destination);
	++*num_of_routes;
	
	return 0;
}

int is_route_available(char * destination, int num_of_routes, struct route * routes) 
{
	int i;
	for (i = 0; i < num_of_routes; i++) 
	{
		if (strcmp(routes[i].destination, destination) == 0)
			return 1;
	}

	return 0;
}