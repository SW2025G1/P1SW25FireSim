#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#define MAX_FORECAST_HOURS 72
#define MAX_MAP_SIZE 40
// Farver til brand-status:
#define RED_BG    "\033[41m"    // Rød baggrund for Udbrændt (Status >= 1.0)
#define GREEN_BG  "\033[42m"    // Grøn baggrund for Uforbrændt (Status == 0.0)
#define YELLOW_BG "\033[43m"    // Gul baggrund for Brændende (0.0 < Status < 1.0)
#define RESET     "\033[0m"     // VIGTIGT: Nulstiller farve og baggrund til standarden

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <bemapiset.h>

typedef struct cell_t {
    double topography;
    char fuel[4];
    double status;
} cell_t;

typedef struct map_t {
    int size_of_map;
    cell_t *map;
    cell_t *temp_map;
} map_t;

typedef struct Weather_t {
    double moisture_of_fuel;
    double wind_speed;
    int wind_direction; //skal laves om til int, fordi vi skal give bruger 8 valgmuligheder i stedet for selv at skrive northeast eller NW
    double wind_direction_radians;
} Weather_t;

void enable_ansi_codes();

FILE* get_file_path_from_user();
void initialize_map(map_t* map);
void get_size_of_map(FILE *fptr, map_t* map);
void get_data_from_file(FILE *fptr, map_t* map);
void print_grid(map_t* map);
void debug_print(map_t* map);
void free_memory(map_t* map);
Weather_t weather_input_from_user();
void initial_burning_cell(map_t* map);


/*typedef struct global_params_t {
    double wind_speed_forecast[MAX_FORECAST_HOURS];
    double wind_direction_forecast[MAX_FORECAST_HOURS];
    double moisture_of_fuel;
}global_params_t;*/
#endif
