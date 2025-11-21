
// Prototype for example function
#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define MAX_FORECAST_HOURS 72
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

typedef struct global_params_t {
    double wind_speed_forecast[MAX_FORECAST_HOURS];
    double wind_direction_forecast[MAX_FORECAST_HOURS];
    double moisture_of_fuel;
}global_params_t;

FILE* open_data(char* file_name);
char* get_file_path_from_user();
void initialize_array(array_t* array);
#endif
