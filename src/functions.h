#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define MAX_FORECAST_HOURS 72
#define MAX_MAP_SIZE 50
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

typedef struct {
    double moisture_of_fuel;
    double wind_speed;
    char wind_direction[20]; //skal laves om til double, fordi vi skal give bruger 8 valgmuligheder i stedet for selv at skrive northeast eller NW
} Weather;

FILE* open_data(char* file_name);
char* get_file_path_from_user();
void initialize_array(array_t* array);
void get_size_of_map(FILE *fptr, array_t* array);
void get_data_from_file(FILE *fptr, array_t* array);
void print_grid(array_t* array);
void debug_print(array_t* array);
void free_memory(array_t* array);
Weather weather_input_from_user();
#endif
