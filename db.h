#ifndef DB_H_INCLUDED
#define DB_H_INCLUDED

#include <stdio.h>

int init_db(FILE ** db, char * db_file_name);
int save_db(FILE * db, int num_of_lines, char ** lines);
int close_db(FILE * db);

#endif
