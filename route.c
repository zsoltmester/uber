#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "route.h"

int add_route(char destination[16], int max_number_of_routes, int *num_of_routes, struct route routes[])
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
	
	strcpy(routes[*num_of_routes].destination, destination);
	//routes[*num_of_routes].passengers = NULL;
	//routes[*num_of_routes].num_of_passengers = 0;
	++*num_of_routes;
	
	return 0;
}

int is_route_available(char destination[16], int num_of_routes, struct route routes[]) 
{
	int i;
	for (i = 0; i < num_of_routes; i++) 
		if (strcmp(routes[i].destination, destination) == 0)
			return 1;

	return 0;
}

int remove_route(char destination[16], int *num_of_routes, struct route routes[])
{
	if (*num_of_routes == 1)
	{
		printf("[ERROR] You should have at least one route.\n");
		return 1;
	}

	if (strlen(destination) == 0) 
	{
		printf("[ERROR] Destination is empty.\n");
		return 1;
	}
	if (is_route_available(destination, *num_of_routes, routes) == 0) 
	{
		printf("[ERROR] No route available for this destination.\n");
		return 1;
	}
	
	int i;
	for (i = 0; i < *num_of_routes; ++i)
		if (strcmp(routes[i].destination, destination) == 0)
		{
			if(i == *num_of_routes - 1)
				break;
				
			int j;
			for (j = i; j < *num_of_routes - 1; ++j) 
				routes[j] = routes[j + 1];
		}
	
	--*num_of_routes;
	
	return 0;
}

/*int add_passenger(char * name, char * phone, time_t reg_time, struct route * route, int max_number_of_passengers)
{
	if (route -> num_of_passengers == max_number_of_passengers) 
	{
		printf("[ERROR] No more passengers allowed.\n");
		return 1;
	}
	if (is_passenger_available(name, route)) 
	{
		printf("[ERROR] This passenger is already exists.\n");
		return 1;
	}
	
	if (route -> num_of_passengers == 0)
		route -> passengers = malloc(sizeof(struct passenger) * max_number_of_passengers);
	
	route -> passengers[route -> num_of_passengers].name = name;
	route -> passengers[route -> num_of_passengers].phone = phone;
	route -> passengers[route -> num_of_passengers].reg_time = reg_time;
	++(route -> num_of_passengers);
	
	return 0;
}

int is_passenger_available(char * name, struct route * route)
{
	int i;
	for(i = 0; i < route -> num_of_passengers; ++i)
		if (strcmp(route -> passengers[i].name, name) == 0)
			return 1;
		
	return 0;
}*/
