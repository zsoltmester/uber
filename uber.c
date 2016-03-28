#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "route.h"

// customization points
#define MAX_NUMBER_OF_ROUTES 10
#define DB_FILE_NAME "uber.db"

void init_db();
int save_db();
int read_db();
void trim(char * str);

struct route * routes;
int num_of_routes;
FILE * db;

int main(int argc, char **argv) 
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
		init_db();
		add_route("Krakkó", MAX_NUMBER_OF_ROUTES, &num_of_routes, routes);
		add_route("Prága", MAX_NUMBER_OF_ROUTES, &num_of_routes, routes);
		add_route("Bécs", MAX_NUMBER_OF_ROUTES, &num_of_routes, routes);
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
	db = freopen(NULL, "w+", db);
	if (db == NULL)
		return 1;
		
	int i;
	for (i = 0; i < num_of_routes; i++) 
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
    	add_route(line, MAX_NUMBER_OF_ROUTES, &num_of_routes, routes);
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
