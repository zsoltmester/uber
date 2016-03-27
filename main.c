#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "route.h"

#define MAX_NUMBER_OF_ROUTES 10

void add_route(char * destination);
int is_route_available(char * destination);

struct route * routes;
int numOfRoutes;

int main(int argc, char **argv) 
{
	routes = malloc(sizeof(struct route) * MAX_NUMBER_OF_ROUTES);
	numOfRoutes = 0;
	
	// add the default routes
	add_route("Krakkó");
	add_route("Prága");
	add_route("Bécs");
	
	return 0;
}

void add_route(char * destination)
{
	if (numOfRoutes == MAX_NUMBER_OF_ROUTES) 
	{
		printf("[ERROR] No more routes allowed.\n");
		return;
	}
	if (strlen(destination) == 0) 
	{
		printf("[ERROR] Destination is empty.\n");
		return;
	}
	if (is_route_available(destination)) 
	{
		printf("[ERROR] This destination is already available.\n");
		return;
	}
	
	routes[numOfRoutes].destination = destination;
	++numOfRoutes;
}

int is_route_available(char * destination) 
{
	int i;
	for (i = 0; i < numOfRoutes; i++) 
	{
		if (strcmp(routes[i].destination, destination) == 0)
			return 1;
	}

	return 0;
}
