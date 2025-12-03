#ifndef SIMULATION_H
#define SIMULATION_H
#include "functions.h"
#include "../visualizer/visualizer.h"

#define TIME_STEP 5 //TODO play around with smaller timesteps?
#define DIRECTIONS_AMOUNT 8
#define CELL_WIDTH 5
#define SQRT_OF_2 1.41421356
#define TL1_MOISTURE_EXTINCTION 0.30
#define TL1_BASE_BASE_RATE 0.064
#define TL1_WIND_SCALING_RATIO 0.15 //Dense fuel packing ratio means lesser wind scaling ratio than TU1
#define TL1_SLOPE_SCALING_RATIO 0.05 //Dense fuel packing reatio means also lesser thermal penetration efficiency uphill
#define TU1_MOISTURE_EXTINCTION 0.20
#define TU1_BASE_BASE_RATE 0.140
#define TU1_WIND_SCALING_RATIO 0.35 //TU1 has a smaller packing ratio meaning the wind effect on fire spread is greater
#define TU1_SLOPE_SCALING_RATIO 0.20

typedef enum cardinal_directions {
    East, NorthEast, North, NorthWest, West, SouthWest, South, SouthEast
} directions_e;

typedef struct direction_from_neighbor_t {
    int direction_from_neighbor_int;
    double direction_from_neighbor_radians;
} direction_t;

void   sim_loop(map_t* map, Weather_t* w);
void   calculate_new_status(map_t* map, Weather_t* w, int i, int j);
double get_neighbor_status(const map_t* map, int i, int j, int direction);
void   input_time_or_exit(char* input_char, int* input_time);
double status_calculator(map_t* map, Weather_t* w, int i, int j, direction_t direction_from_neighbor);
double calculate_base_rate(map_t* map, Weather_t* w, int i, int j, direction_t direction_from_neighbor);
void   update_base_rate_values(map_t* map, double* base_base_rate, double* extinction_moisture_of_cell, int i, int j);
double direction_to_radians(int wind_direction);
double calculate_wind_factor(map_t* map, int i, int j, Weather_t* w, direction_t direction_from_neighbor);
double get_wind_scaling_for_fuel_model(map_t* map, int i, int j);
double calculate_slope_factor(map_t* map, int i, int j, direction_t direction_from_neighbor);
double get_slope_scaling_for_fuel_model(map_t* map, int i, int j);
double get_neighbour_elevation(map_t* map, int i, int j, direction_t direction_from_neighbor);
double calculate_total_spread_rate(double base_rate_of_spread, double wind_factor, double slope_factor);
int    convert_input_to_time(int* input_time);
int    get_neighbor_index(const map_t* map, int i, int j, int direction);
void   update_timekeeper(int input_time, int* all_time);

#endif //SIMULATION_H