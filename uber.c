#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "route.h"
#include "db.h"

// customization points
#define MAX_NUMBER_OF_ROUTES 10
#define DB_FILE_NAME "uber.db"

struct route * routes;
int num_of_routes;
FILE * db;

int main(int argc, char ** argv) 
{
	// TODO process args

	routes = malloc(sizeof(struct route) * MAX_NUMBER_OF_ROUTES);
	num_of_routes = 0;

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
		printf("%s is not available, it will be created with the default values...\n", DB_FILE_NAME);
		init_db(&db, DB_FILE_NAME);
		printf("%s sucessfully created.\n", DB_FILE_NAME);
		
		add_route("Krakkó", MAX_NUMBER_OF_ROUTES, &num_of_routes, routes);
		add_route("Prága", MAX_NUMBER_OF_ROUTES, &num_of_routes, routes);
		add_route("Bécs", MAX_NUMBER_OF_ROUTES, &num_of_routes, routes);
	}
	
	// TODO do the task
	
	// save the db
	char ** lines = malloc(sizeof(char *) * num_of_routes);
	int i;
	for (i = 0; i < num_of_routes; i++) 
	{
		lines[i] = malloc(strlen(routes[i].destination));
		strcpy(lines[i], routes[i].destination);
	}
	if (save_db(db, num_of_routes, lines) && close_db(db))
	{
		perror("[ERROR] Failed to save the database.\n");
		return 1;
	}
	
	// TODO free up some memory
	
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
			return 1;
    	}
    	add_route(line, MAX_NUMBER_OF_ROUTES, &num_of_routes, routes);
    }
	
	if (line)
		free(line);
	
	return 0;
}
