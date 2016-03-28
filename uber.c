#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "route.h"
#include "db.h"

// customization points
#define MAX_NUMBER_OF_ROUTES 10
#define DB_FILE_NAME "uber.db"

enum cmd 
{
	HELP, LIST_ROUTES
};

struct task
{
	enum cmd cmd;
	int num_of_args;
	char ** args;
};

void free_array(void ** arr, int len);
void init_state();
void save_state();
void clear();
struct task * parse_args(int argc, char ** argv);
void do_task(struct task * task);
void task_help();
void task_list_routes();

struct route * routes;
int num_of_routes;
FILE * db;

int main(int argc, char ** argv) 
{
	struct task * task = parse_args(argc, argv);
	init_state();
	do_task(task);
	free(task);
	save_state();
	clear();
	return 0;
}

void free_char_array(char ** arr, int len)
{
	if (arr)
	{
		int i = 0;
		for(i = 0; i < len; ++i)
			if(arr[i])
				free(arr[i]);
		free(arr);
	}
}

void init_state()
{
	routes = malloc(sizeof(struct route) * MAX_NUMBER_OF_ROUTES);
	num_of_routes = 0;

	if (access(DB_FILE_NAME, W_OK) == 0) 
	{
		db = fopen(DB_FILE_NAME, "r+");
		if (db == NULL)
		{ 
			printf("[ERROR] Unable to open the database.\n");
			clear();
			exit(1);
		}
		
		int num_of_lines = 0;
		char ** lines = read_db(db, MAX_NUMBER_OF_ROUTES, &num_of_lines);
		if (lines == NULL)
		{ 
			printf("[ERROR] Unable to read the database.\n");
			free_char_array(lines, num_of_lines);
			clear();
			exit(1);
		}
		
		if (parse_lines(lines, num_of_lines, MAX_NUMBER_OF_ROUTES, &num_of_routes, routes))
		{
			printf("[ERROR] Unable to parse the database.\n");
			free_char_array(lines, num_of_lines);
			clear();
			exit(1);
		}
		
		free_char_array(lines, num_of_lines);
	}
	else 
	{
		printf("%s is not available, it will be created with the default values...\n", DB_FILE_NAME);
		init_db(&db, DB_FILE_NAME);
		if (db == NULL)
		{
			printf("[ERROR] Unable to create the database.\n");
			clear();
			exit(1);
		}
		printf("%s sucessfully created.\n", DB_FILE_NAME);
		
		add_route("Krakkó", MAX_NUMBER_OF_ROUTES, &num_of_routes, routes);
		add_route("Prága", MAX_NUMBER_OF_ROUTES, &num_of_routes, routes);
		add_route("Bécs", MAX_NUMBER_OF_ROUTES, &num_of_routes, routes);
	}
}

void save_state()
{
	char ** lines = routes_to_lines(num_of_routes, routes);
	if (save_db(db, num_of_routes, lines) && close_db(db))
	{
		printf("[ERROR] Failed to save the database.\n");
		return;
	}
	free_char_array(lines, num_of_routes);
}

void clear()
{
	if (db)
		fclose(db);
	
	if (routes == NULL)
		return;
	 
	int i;
	for (i = 0; i < num_of_routes; ++i)
		if (routes[i].destination) 
			free(routes[i].destination);
	free(routes);
}

struct task * parse_args(int argc, char ** argv)
{
	struct task * task = malloc(sizeof(task));
	
	if (argc < 2)
	{
		task -> cmd = HELP;
		return task;
	}
		
	char * raw_cmd = argv[1];
	
	if (strcmp(raw_cmd, "help") == 0)
	{
		task -> cmd = HELP;
	}	
	else if (strcmp(raw_cmd, "list-routes") == 0)
	{
		task -> cmd = LIST_ROUTES;
	}
	else
	{
		printf("Invalid command: %s\n", raw_cmd);
		task -> cmd = HELP;
	}
	
	return task;
}

void do_task(struct task * task)
{
	switch(task -> cmd)
	{
		case HELP:
			task_help();
			break;
		case LIST_ROUTES:
			task_list_routes();
			break;
	}
}

void task_help()
{
	puts("NAME\n\tuber - Best of ŰBER - A passenger management tool.\n\
SYNOPSIS\n\tuber <command> [<args>]\n\
COMMANDS\n\
\thelp - This help.\n\
\tlist-routes - List of the available routes.\n");
}

void task_list_routes()
{
	int i;
	for (i = 0; i < num_of_routes; ++i)
		printf("%s\n", routes[i].destination);
}
