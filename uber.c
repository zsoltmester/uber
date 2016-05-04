#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#include "configuration.h"
#include "feedback.h"
#include "passenger.h"
#include "route.h"

enum cmd
{
	HELP,
	LIST_ROUTES,
	ADD_ROUTE,
	REMOVE_ROUTE,
	ADD_PASSENGER,
	REMOVE_PASSENGER,
	MODIFY_PASSENGER,
	TAKE_ROUTE
};

struct task
{
	enum cmd cmd;
	int num_of_args;
	char ** args;
};

void init_state();
void save_db();
struct task * parse_args(int argc, char ** argv);
void do_task(struct task * task);
void task_help();
void task_list_routes();
void task_add_route(struct task * task);
void task_remove_route(struct task * task);
void task_add_passenger(struct task * task);
void task_remove_passenger(struct task * task);
void task_modify_passenger(struct task * task);
void task_take_route(struct task * task);
void manage_route(pid_t pid, int pipefd[2], struct route *route);
void guide_route(int pipefd[2], struct route *route);

struct route routes[MAX_NUMBER_OF_ROUTES];
int num_of_routes = 0;
FILE * db;

int main(int argc, char ** argv)
{
	struct task * task = parse_args(argc, argv);
	init_state();
	do_task(task);
	save_db();
	return 0;
}

void init_state()
{
	if (access(DB_FILE_NAME, W_OK) == 0)
	{
		db = fopen(DB_FILE_NAME, "rb");
		if (db == NULL)
		{
			printf("[ERROR] Unable to open the database.\n");
			exit(1);
		}

		num_of_routes = fread(routes, sizeof(struct route), MAX_NUMBER_OF_ROUTES, db);
		if (routes == NULL || num_of_routes == 0) {
			printf("[ERROR] Unable to read the database.\n");
			exit(1);
		}
	}
	else
	{
		printf("%s is not available, it will be created with the default values...\n", DB_FILE_NAME);
		db = fopen(DB_FILE_NAME, "wb+");
		if (db == NULL)
		{
			printf("[ERROR] Unable to create the database.\n");
			exit(1);
		}
		printf("%s sucessfully created.\n", DB_FILE_NAME);

		add_route("Krakkó", &num_of_routes, routes);
		add_route("Prága", &num_of_routes, routes);
		add_route("Bécs", &num_of_routes, routes);
	}
}

void save_db()
{
	db = freopen(NULL, "wb", db);
	if (fwrite(routes, sizeof(struct route), num_of_routes, db) != num_of_routes
			|| fclose(db) != 0)
		printf("[ERROR] Failed to save the database.\n");
}

struct task * parse_args(int argc, char ** argv)
{
	struct task * task = malloc(sizeof(struct task *));

	if (argc < 2)
	{
		task->cmd = HELP;
		return task;
	}

	task->num_of_args = argc;
	if (task->num_of_args > 2)
	{
		task->args = argv;
	}

	char * raw_cmd = argv[1];

	if (strcmp(raw_cmd, "help") == 0)
	{
		task->cmd = HELP;
	}
	else if (strcmp(raw_cmd, "list-routes") == 0)
	{
		task->cmd = LIST_ROUTES;
	}
	else if (strcmp(raw_cmd, "add-route") == 0)
	{
		task->cmd = ADD_ROUTE;
	}
	else if (strcmp(raw_cmd, "remove-route") == 0)
	{
		task->cmd = REMOVE_ROUTE;
	}
	else if (strcmp(raw_cmd, "add-passenger") == 0)
	{
		task->cmd = ADD_PASSENGER;
	}
	else if (strcmp(raw_cmd, "remove-passenger") == 0)
	{
		task->cmd = REMOVE_PASSENGER;
	}
	else if (strcmp(raw_cmd, "modify-passenger") == 0)
	{
		task->cmd = MODIFY_PASSENGER;
	}
	else if (strcmp(raw_cmd, "take-route") == 0)
	{
		task->cmd = TAKE_ROUTE;
	}
	else
	{
		printf("Invalid command: %s\n", raw_cmd);
		task->cmd = HELP;
	}

	return task;
}

void do_task(struct task * task)
{
	switch(task->cmd)
	{
		case HELP:
			task_help();
			break;
		case LIST_ROUTES:
			task_list_routes();
			break;
		case ADD_ROUTE:
			task_add_route(task);
			break;
		case REMOVE_ROUTE:
			task_remove_route(task);
			break;
		case ADD_PASSENGER:
			task_add_passenger(task);
			break;
		case REMOVE_PASSENGER:
			task_remove_passenger(task);
			break;
		case MODIFY_PASSENGER:
			task_modify_passenger(task);
			break;
			break;
		case TAKE_ROUTE:
			task_take_route(task);
			break;
	}
}

void task_help()
{
	puts("NAME\n\tuber - Best of Über - A passenger management tool.\n\
SYNOPSIS\n\tuber <command> [<args>]\n\
COMMANDS\n\
\thelp - This help.\n\
\tlist-routes - List of the available routes.\n\
\tadd-route - Add a route. \n\t\tARGS:\n\
\t\t1. Destination of the new route.\n\
\tremove-route - Remove a route. \n\t\tARGS:\n\
\t\t1. Destination of the route to remove.\n\
\tadd-passenger - Add a passenger. \n\t\tARGS:\n\
\t\t1. Destination of the route.\n\
\t\t2. Name.\n\
\t\t2. Phone number.\n\
\tremove-passenger - Remove a passenger. \n\t\tARGS:\n\
\t\t1. Destination of the route.\n\
\t\t2. Name.\n\
\tmodify-passenger - Modify a passenger. \n\t\tARGS:\n\
\t\t1. Current destination of the route.\n\
\t\t2. Current name.\n\
\t\t3. New destination. In case of no change, type =\n\
\t\t4. New name. In case of no change, type =\n\
\t\t5. New phone. In case of no change, type =\n\
\ttake-route - Take a route. \n\t\tARGS:\n\
\t\t1. Destination of the route.\n");
}

void task_list_routes()
{
	int i;
	for (i = 0; i < num_of_routes; ++i)
	{
		printf("%s\n", routes[i].destination);
		int j;
		for (j = 0; j < routes[i].num_of_passengers; ++j)
			printf("\tname: %s, phone: %s, last modification time: %s",
				routes[i].passengers[j].name,
				routes[i].passengers[j].phone,
				ctime(&(routes[i].passengers[j].mod_time)));
	}
}

void task_add_route(struct task * task)
{
	if (task->num_of_args != 3)
	{
		puts("[ERROR] Invalid number of args. It must be 1, which is the destination.");
		return;
	}

	add_route(task->args[2], &num_of_routes, routes);
}

void task_remove_route(struct task * task)
{
	if (task->num_of_args != 3)
	{
		puts("[ERROR] Invalid number of args. It must be 1, which is the destination.");
		return;
	}

	remove_route(task->args[2], &num_of_routes, routes);
}

void task_add_passenger(struct task * task)
{
	if (task->num_of_args != 5)
	{
		puts("[ERROR] Invalid number of args. It must be 3.");
		return;
	}

	int i;
	for (i = 0; i < num_of_routes; ++i)
		if (strcmp(routes[i].destination, task->args[2]) == 0)
		{
			add_passenger(task->args[3], task->args[4], &routes[i]);
			return;
		}

	puts("[ERROR] No route found for the given destination.");
}

void task_remove_passenger(struct task * task)
{
	if (task->num_of_args != 4)
	{
		puts("[ERROR] Invalid number of args. It must be 2.");
		return;
	}

	remove_passenger(task->args[2], task->args[3], num_of_routes, routes);
}

void task_modify_passenger(struct task * task)
{
	if (task->num_of_args != 7)
	{
		puts("[ERROR] Invalid number of args. It must be 5.");
		return;
	}

	modify_passenger(task->args[2], task->args[3], task->args[4], task->args[5], task->args[6], num_of_routes, routes);
}

void task_take_route(struct task * task)
{
	if (task->num_of_args != 3)
	{
		puts("[ERROR] Invalid number of args. It must be 1.");
		return;
	}

	// Process the only argument.
	struct route *route = get_route(task->args[2], num_of_routes, routes);
	if (route != NULL)
	{
		printf("Take a route to %s...\n", route -> destination);
	}
	else
	{
		printf("[ERROR] No route to %s.\n", route -> destination);
		return;
	}

	// Create the pipe.
	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		printf("[ERROR] Cannot instantiate a pipe.\n");
		return;
	}

	// Create the child process, which will be the 'courier'.
	pid_t pid = fork();
	if (pid < 0)
	{
		printf("[ERROR] Cannot fork.\n");
		return;
	}
	else if (pid > 0)
	{
		// Parent process, which is the 'über'.
		manage_route(pid, pipefd, route);
	}
	else
	{
		// Child process, which is the 'courier'.
		guide_route(pipefd, route);
	}
}

void manage_route(pid_t pid, int pipefd[2], struct route *route)
{
	// Close the writing end.
	close(pipefd[1]);

	struct feedback fb;

	// Waiting for the results.
	puts("The Über is waiting for the results...");
	read(pipefd[0], &fb, sizeof(fb));
	close(pipefd[0]);

	// Write out the results.
	int i;
	for (i = 0; i < fb.num_of_passengers; ++i)
	{
		printf("%s answer: %d\n", fb.passengers[i].name, fb.values[i]);
	}

	// Sends a signal to the courier that the result are arrived.
	puts("As the Über, I am sending a signal to the courier that the result are arrived...");
	kill(pid, SIGTERM);

	// Wait for the courier and terminate.
	int status;
    waitpid(pid, &status, 0);
	puts("The courier terminated and the Über is terminating now...");
}

void guide_route(int pipefd[2], struct route *route)
{
	// Close the reading end.
	close(pipefd[0]);

	// Guide the route.
	puts("The route is started...");
	int i;
	for (i = 0; i < ROUTE_LENGTH; ++i)
	{
		printf("Remaining time from the route: %d\n", ROUTE_LENGTH - i);
		sleep(1);
	}

	// Generate and write out the feedbacks.
	struct feedback fb;
  	srand(time(NULL));
	fb.num_of_passengers = route -> num_of_passengers;
	for (i = 0; i < route -> num_of_passengers; ++i)
	{
		fb.passengers[i] = (route -> passengers)[i];
		fb.values[i] = 1 + rand() % 5;
	}
	write(pipefd[1], &fb, sizeof(fb));
	close(pipefd[1]);

	// Waiting for the response for the Über, that the results arrived.
	puts("As a courier, I am waiting for the termination signal from the Über...");
	pause();
}
