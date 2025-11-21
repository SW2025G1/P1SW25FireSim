
// Prototype for example function
#ifndef functions.h
#define functions.h
#include <stdio.h>
#include <stdlib.h>

typedef struct cell_t {
    double topography;
    char fuel[4];
    double status;
} cell_t;

typedef struct array_t {
    int size_of_array;
    cell_t *map;
} array_t;



FILE* open_data(char* file_name);
char* get_file_path_from_user();
void initialize_array(array_t* array);
#endif
