#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "constants-and-enum.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for usleep in the progress bar function

#ifdef _WIN32
#include <windows.h>
#include <bemapiset.h>
#endif

/**
 * a struct that read the parameters from the data files
 */
typedef struct cell_t {
    double topography;
    char fuel[4];
    double status;
} cell_t;

/**
 * Struct to find the size of map
 */
typedef struct map_t {
    int size_of_map;
    cell_t *map;
    cell_t *temp_map;
} map_t;

/**
 * This struct contains the conditions of weather
 */
typedef struct Weather_t {
    double moisture_of_fuel;
    double wind_speed;
    int wind_direction;
    double wind_direction_radians;
} Weather_t;

void enable_ansi_codes(void);

FILE* get_file_path_from_user(void);
void initialize_map(map_t* map);
void get_size_of_map(FILE *fptr, map_t* map);
void get_data_from_file(FILE *fptr, map_t* map);
void print_grid(map_t* map);
void debug_print(map_t* map);
void free_memory(map_t* map);
Weather_t weather_input_from_user(void);
void initial_burning_cell(map_t* map);
void input_time_or_exit(int* input_time);
void print_progress(double k,  int input_time);

#endif
