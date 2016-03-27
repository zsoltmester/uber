#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "route.h"

// customization points
#define MAX_NUMBER_OF_ROUTES 10
#define DB_FILE_NAME "uber.db"

void add_route(char * destination);
int is_route_available(char * destination);
void init_db();
int save_db();
int read_db();
void trim(char * str);

struct route * routes;
int numOfRoutes;
FILE * db;

int main(int argc, char **argv) 
{
	// TODO process args

	routes = malloc(sizeof(struct route) * MAX_NUMBER_OF_ROUTES);
	numOfRoutes = 0;

	if (access(DB_FILE_NAME, W_OK) == 0) 
	{
		db = fopen(DB_FILE_NAME, "r+");
		if (db == NULL)
		{ 
			perror("[ERROR] Unable to open the database.\n");
			exit(1);
		}
		if (read_db())
		{ 
			perror("[ERROR] Unable to read the database.\n");
			exit(1);
		}
	}
	else 
	{
		// first run or removed db. we should initialize it
		init_db();
		add_route("Krakkó");
		add_route("Prága");
		add_route("Bécs");
	}
	
	// TODO do the task
	
	if (save_db())
	{
		perror("[ERROR] Failed to save the database.\n");
		free(routes);
		exit(1);
	}
	free(routes);
	
	return 0;
}

void init_db()
{
	printf("uber.db is not available, it will be created with the default values...\n");
	db = fopen(DB_FILE_NAME, "w+");
	if (db == NULL)
	{ 
		perror("[ERROR] Unable to create the database.\n");
		exit(1);
	}
	printf("uber.db sucessfully created.\n");
}

int save_db()
{
	if (ftruncate(fileno(db), 0))
		return 1;
		
	int i;
	for (i = 0; i < numOfRoutes; i++) 
	{
		if(fprintf(db, "%s\n", routes[i].destination) <= 0)
			return 1;
	}
	
	return fclose(db);
}

int read_db()
{
	char * line = NULL;
    size_t len = 0;
    
    while (getline(&line, &len, db) != -1) 
    {
    	trim(line);
    	if (strlen(line) == 0)
    	{
			printf("[ERROR] Corrupt db file.\n");
			exit(1);
    	}
    	add_route(line);
    }
	
	if (line)
		free(line);
	
	return 0;
}

void trim(char * str) 
{
    int len = strlen(str);
    if (len == 0)
    	return;
    
    char * trimmed = str;

	// trim the whitespaces from the end
    while(isspace(trimmed[len - 1])) 
    {
    	--len;
    	trimmed[len] = 0;
    }
    
    // trim the whitespaces from the start
    while(*trimmed && isspace(*trimmed)) 
    {
    	--len;
    	++trimmed;
    }

    memmove(str, trimmed, len + 1);
}

void add_route(char * destination)
{
	if (numOfRoutes == MAX_NUMBER_OF_ROUTES) 
	{
		printf("[ERROR] No more routes allowed.\n");
		exit(1);
	}
	if (strlen(destination) == 0) 
	{
		printf("[ERROR] Destination is empty.\n");
		exit(1);
	}
	if (is_route_available(destination)) 
	{
		printf("[ERROR] This destination is already available.\n");
		exit(1);
	}
	
	routes[numOfRoutes].destination = malloc(strlen(destination));
	strcpy(routes[numOfRoutes].destination, destination);
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
