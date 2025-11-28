#ifndef SIMULATION_H
#define SIMULATION_H
#include "functions.h"
/*
typedef struct TimberLitter1 {
    double Pb = 24.975;
    double effective_heating_number = 0.9227;
    double Q_ig = 788.520;
    double flux_ratio = 0.1174;
    double reaction_intenset = 1.500;
} TimberLitter1;*/

typedef enum cardinal_directions {
    East, NorthEast, North, NorthWest, West, SouthWest, South, SouthEast
} directions_e;

typedef struct direction_from_neighbor_t {
    int direction_from_neighbor_int;
    double direction_from_neighbor_radians;
} direction_t;

void   sim_loop(map_t* map, Weather_t* w);
void   calculate_new_status(map_t* map, map_t* temp_map, Weather_t* w, int i, int j);
void   input_time_or_exit(char* input_char, int* input_time);
double status_calculator(map_t* map, Weather_t* w, int i, int j, direction_t direction_from_neighbor);
double calculate_base_rate(map_t* map, Weather_t* w, int i, int j, direction_t direction_from_neighbor);
void   update_base_rate_values(map_t* map, double* base_base_rate, double* extinction_moisture_of_cell, int i, int j);
double direction_to_radians(int wind_direction);
double calculate_wind_factor(map_t* map, int i, int j, Weather_t* w, direction_t direction_from_neighbor);
double get_wind_scaling_for_fuel_model(map_t* map, int i, int j);
double calculate_slope_factor(map_t* map, int i, int j, direction_t direction_from_neighbor);
double get_neighbour_elevation(map_t* map, int i, int j, direction_t direction_from_neighbor);
double calculate_total_spread_rate(double base_rate_of_spread, double wind_factor, double slope_factor);

#endif //SIMULATION_H