#ifndef VACCINATION_FILE
#define VACCINATION_FILE

#include "vaccination.c"

bool isEmptyFile(char *fileName);
void add_new_data(char *fileName);
void modify_data(char *fileName);
void delete_data(char *filename);
void create_total_list(char *fileName);
void print_date(char *fileName);


#endif // VACCINATION_FILE