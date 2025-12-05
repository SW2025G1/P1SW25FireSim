#ifndef SIMULATION_H
#define SIMULATION_H
#include <time.h>
#include "input-output.h"
#include "../visualizer/visualizer.h"

typedef enum cardinal_directions {
    East, NorthEast, North, NorthWest, West, SouthWest, South, SouthEast
} directions_e;

typedef struct direction_from_neighbor_t {
    int direction_from_neighbor_int;
    double direction_from_neighbor_radians;
} direction_t;

void   sim_loop(map_t* map, Weather_t* w);
void   calculate_new_status(map_t* map, Weather_t* w, int i, int j);
double status_calculator(map_t* map, Weather_t* w, int i, int j, direction_t direction_from_neighbor);
double calculate_base_rate(map_t* map, Weather_t* w, int i, int j);
void   update_base_rate_values(map_t* map, double* base_base_rate, double* extinction_moisture_of_cell, int i, int j);
double direction_to_radians(int wind_direction);
double calculate_wind_factor(map_t* map, int i, int j, Weather_t* w, direction_t direction_from_neighbor);
double get_wind_scaling_for_fuel_model(map_t* map, int i, int j);
double calculate_slope_factor(map_t* map, int i, int j, direction_t direction_from_neighbor);
double get_slope_scaling_for_fuel_model(map_t* map, int i, int j);
double calculate_total_spread_rate(double base_rate_of_spread, double wind_factor, double slope_factor);
int    convert_input_to_time(int* input_time);
int    get_neighbor_index(const map_t* map, int i, int j, int direction);
int    is_diagonal(int direction);
void   update_timekeeper(int input_time, int* all_time);
double runtime_end(clock_t begin);
#endif //SIMULATION_H