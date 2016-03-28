#include <stdlib.h>
#include <string.h>

#include "db.h"

void init_db(FILE ** db, char * db_file_name) 
{
	*db = fopen(db_file_name, "w+");
}

int save_db(FILE * db, int num_of_lines, char ** lines)
{
	db = freopen(NULL, "w+", db);
	if (db == NULL)
		return 1;
		
	int i;
	for (i = 0; i < num_of_lines; i++) 
	{
		if(fprintf(db, "%s\n", lines[i]) <= 0)
			return 1;
	}
	
	return 0;
}

int close_db(FILE * db)
{
	return fclose(db);
}

char ** read_db(FILE * db, int max_number_of_lines, int * num_of_lines)
{
	char ** lines = malloc(sizeof(char *) * max_number_of_lines);
	char * line = NULL;
    size_t len = 0;
    
    *num_of_lines = 0;
    while (getline(&line, &len, db) != -1) 
    {
    	line[strlen(line) - 1] = 0;
    	lines[*num_of_lines] = malloc(strlen(line));
    	strcpy(lines[*num_of_lines], line);
    	++*num_of_lines;
    }
	
	if (line)
		free(line);
	
	return lines;
}
