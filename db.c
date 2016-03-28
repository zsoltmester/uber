#include "db.h"

int init_db(FILE ** db, char * db_file_name) 
{
	*db = fopen(db_file_name, "w+");
	if (*db == NULL)
	{ 
		printf("[ERROR] Unable to create the database.\n");
		return 1;
	}
	return 0;
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
