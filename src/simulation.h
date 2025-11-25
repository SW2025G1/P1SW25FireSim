#ifndef SIMULATION_H
#define SIMULATION_H
#include "functions.h"

void sim_loop(array_t* array, Weather* w);
void calculate_new_status(array_t* array, Weather* w, int i, int j, int k);
void input_time_or_exit(char* input_char, int* input_time);
double rate_of_spread(array_t* array, Weather* w, int i, int j, int k);
double calculate_base_rate(char* fuel_model, double moisture_of_fuel);
double calculate_wind_factor(int k, double wind_speed, double wind_direction);
double calculate_slope_factor(int k, int i, int j);
double get_neighbour_elevation(int k, int i, int j);
double calculate_total_spread_rate(double base_rate_of_spread, double wind_factor, double slope_factor);

#endif //SIMULATION_H