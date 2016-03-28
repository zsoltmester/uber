#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
	char ** lines;
	int num_of_lines;

	if (access(DB_FILE_NAME, W_OK) == 0) 
	{
		db = fopen(DB_FILE_NAME, "r+");
		if (db == NULL)
		{ 
			printf("[ERROR] Unable to open the database.\n");
			return 1;
		}
		
		num_of_lines = 0;
		lines = read_db(db, MAX_NUMBER_OF_ROUTES, &num_of_lines);
		if (lines == NULL)
		{ 
			printf("[ERROR] Unable to read the database.\n");
			return 1;
		}
		
		if (parse_lines(lines, num_of_lines, MAX_NUMBER_OF_ROUTES, &num_of_routes, routes))
		{
			printf("[ERROR] Unable to parse the database.\n");
			return 1;
		}
	}
	else 
	{
		printf("%s is not available, it will be created with the default values...\n", DB_FILE_NAME);
		init_db(&db, DB_FILE_NAME);
		if (db == NULL)
		{
			printf("[ERROR] Unable to create the database.\n");
			return 1;
		}
		printf("%s sucessfully created.\n", DB_FILE_NAME);
		
		add_route("Krakkó", MAX_NUMBER_OF_ROUTES, &num_of_routes, routes);
		add_route("Prága", MAX_NUMBER_OF_ROUTES, &num_of_routes, routes);
		add_route("Bécs", MAX_NUMBER_OF_ROUTES, &num_of_routes, routes);
	}
	
	// TODO do the task
	
	// save the db
	lines = routes_to_lines(num_of_routes, routes);
	if (save_db(db, num_of_routes, lines) && close_db(db))
	{
		printf("[ERROR] Failed to save the database.\n");
		return 1;
	}
	
	// TODO free up some memory
	
	return 0;
}

/*void trim(char * str) 
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
}*/
