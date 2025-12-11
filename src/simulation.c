#include "simulation.h"
#include "input-output.h"

/**
 * This is the simulation loop, where we in this specific function have a loop to generate each timestep of the simulation
 * We also call the helper functions in order to gather all of the simulations parts of the program to this one called in main
 * @param map
 * @param w
 */
void sim_loop(map_t* map, Weather_t* w) {
    int input_time = 0;
    int all_time = 0;
    long long map_size_bytes = (long long)map->size_of_map * (long long)map->size_of_map * sizeof(cell_t);
    memcpy(map->temp_map, map->map, map_size_bytes);
    int simulation_run_count = 0;


    do {
        input_time_or_exit(&input_time);
        clock_t begin = clock();

        if (input_time != 0) {
            for (double k = TIME_STEP; k < input_time; k += TIME_STEP) {
                print_progress(k, input_time);
                for (int i = 1; i < map->size_of_map - 1; i++) { //i is initialized 1, to ensure that it skips the first column
                    for (int j = 1; j < map->size_of_map - 1; j++) {
                        calculate_new_status(map, w, i, j);
                    }
                }
                //Copy all bytes from map->temp_map data area to map->map data area for each time step
                memcpy(map->map, map->temp_map, map_size_bytes);
            }
        }

        double time_spent = runtime_end(begin);
        print_grid(map);
        update_timekeeper(input_time, &all_time);
        simulation_run_count++;

        if (map->size_of_map >= 36) { //If the map size is too large to display nicely in terminal output, write html
            output_and_open_html(simulation_run_count,  map, time_spent, input_time, all_time);
        }

    } while (input_time != 0);
}


/**
 * This calculates the affect each neighbor has to the cell currently being calculated
 */
void calculate_new_status(map_t* map, Weather_t* w, int i, int j) {
    for (int direction = East; direction < DIRECTIONS_AMOUNT; direction ++) { //Angle we calculate is 0 as the starting point - we then ad one fourth to each time (thereby starting in: direction_from_neighbor: East)
        direction_t neighbor_direction;
        neighbor_direction.direction_from_neighbor_int = direction;     //The enum type corresponds to the integer values 0-7 from 0: East to 7: SouthEast
        neighbor_direction.direction_from_neighbor_radians = direction * (M_PI / 4); //These enum types match the actual radians conversions by this operation

        map->temp_map[i * map->size_of_map + j].status += status_calculator(map, w, i, j, neighbor_direction); //logic 1
        }
}

/**
 * This function also combine the helper functions returning either the updated status or 0 if the cell is still not calculated to be in a scenario where it is on fire
 */
double status_calculator(map_t* map, Weather_t* w, int i, int j, direction_t neighbor_direction) {
    int neighbor_dir = neighbor_direction.direction_from_neighbor_int;
    int neighbor_index = get_neighbor_index(map, i, j, neighbor_dir);
    double status_update = 0;

    if (map->map[neighbor_index].status >= 1.0) { //calculate spread if the neighbor is burning
        double distance_between_centers = CELL_WIDTH;
        if (is_diagonal(neighbor_direction.direction_from_neighbor_int) ) {//Odd direction int values are diagonals
            distance_between_centers *= SQRT_OF_2;                         //diagonals are further apart by *= sqrt(2)
        }


        double base_rate_of_spread = calculate_base_rate(map, w, i, j);

        if (base_rate_of_spread > 0.0) { //if the base rate was 0 due to moisture extinction do not calculate onwards
            double wind_factor = calculate_wind_factor(map, i, j, w, neighbor_direction);
            double slope_factor = calculate_slope_factor(map, i, j, neighbor_direction);
            double total_spread_rate = calculate_total_spread_rate(base_rate_of_spread, wind_factor, slope_factor);
            double ignition_time = distance_between_centers / total_spread_rate; //Time to ignition calculated

            status_update = TIME_STEP / ignition_time; //Ratio of progress to ignition (0 no progress, 1 ignited)
        }
        return status_update;
    }
    else return 0.0; //if neighbor is not burning, no progress was made toward ignition
}

/**
 * @param direction The integer value for the direction from the neighbor cell to the current cell
 * @return 1 is true, meaning if the direction is an odd integer, the direction is diagonal, else it is cardinal (0)
 */
int is_diagonal(const int direction) {
    if (direction % 2 == 1) { //if the int % 2 == 1, it is an odd.
        return 1;
    }
    else return 0;
}

/**
 * This function calculates what each direction is in order to use it for the calculation of each neighbor as seen just above
 * @return is the EXIT_FAILURE
 */
int get_neighbor_index(const map_t* map, const int i, const int j, const int direction) {
    switch (direction) {
        case East:      return i * map->size_of_map + (j + 1);
        case NorthEast: return (i - 1) * map->size_of_map + (j + 1);
        case North:     return (i - 1) * map->size_of_map + j;
        case NorthWest: return (i - 1) * map->size_of_map + (j - 1);
        case West:      return i * map->size_of_map + (j - 1);
        case SouthWest: return (i + 1) * map->size_of_map + (j - 1);
        case South:     return (i + 1) * map->size_of_map + j;
        case SouthEast: return (i + 1) * map->size_of_map + (j + 1);
        default: {
            printf("There was some error returning neighbor status, exiting :(\n");
            exit(EXIT_FAILURE);
        }
    }
}

/**
 * This takes the Base rate calculation and find it in the simplified version studied in the repport
 * The function needs fuel model id and moisture fra weather
*/
double calculate_base_rate(map_t* map, Weather_t* w, int i, int j) {
    double extinction_moisture_of_cell; //set to safe value in case assigning values fails in below.
    double base_base_rate; //The base rate not modified for moisture
    update_base_rate_values(map, &base_base_rate, &extinction_moisture_of_cell, i, j);

    return base_base_rate * (1 - (w->moisture_of_fuel / extinction_moisture_of_cell));
}

/**
 * This calculates the total spread rate as found simplified and accounted for in the repport
*/
double calculate_total_spread_rate(double base_rate_of_spread, double wind_factor, double slope_factor) {
    return base_rate_of_spread * (1 + wind_factor + slope_factor);
}

/**
 * The function below takes the calculated base rate and further calculates for the two scenarios TL1 and TU1
 */
void update_base_rate_values(map_t* map, double* base_base_rate, double* extinction_moisture_of_cell, int i, int j) {
    if (strcmp(map->map[i * map->size_of_map + j].fuel, "TL1") == 0) {
        *extinction_moisture_of_cell = TL1_MOISTURE_EXTINCTION;
        *base_base_rate = TL1_BASE_BASE_RATE;

    }
    else if (strcmp(map->map[i * map->size_of_map + j].fuel, "TU1") == 0) {
        *extinction_moisture_of_cell = TU1_MOISTURE_EXTINCTION;
        *base_base_rate = TU1_BASE_BASE_RATE;
    }
    else if (strcmp(map->map[i * map->size_of_map + j].fuel, "GR1") == 0) {
        *extinction_moisture_of_cell = GR1_MOISTURE_EXTINCTION;
        *base_base_rate = GR1_BASE_BASE_RATE;
    }
    else {
        printf("There was an error with identifying the fuel model for the cell! exiting...\n");
        exit (EXIT_FAILURE);
    }
}

/**
*@brief  Calculating the wind factor to find how fast the fire spread from the neighbor cell (direction) to the current cell. Takes into account the maximum rate factor possible by the top off value wind speed by get_max_rate_for_fuel_model.
 *
 * @return  fmax() = which takes the maximal value between two scenarios - If the right hand side is positive = this is the chosen one, but if it is negativ then = 0
 */
double calculate_wind_factor(map_t* map, int i, int j, Weather_t* w, direction_t neighbor_direction) {
    double C_wind = get_wind_scaling_for_fuel_model(map, i, j);
    double max_wind_speed_to_calculate = get_max_wind_speed_for_fuel_model(map, i, j);
    double wind_speed_maxed = fmin(w->wind_speed, max_wind_speed_to_calculate);
    double wind_factor = C_wind * wind_speed_maxed * cos(
                             w->wind_direction_radians - neighbor_direction.direction_from_neighbor_radians);

    return fmax(0,  wind_factor);
}

/**
 * @brief Checks the fuel model ID in the current cell and returns the maximal wind speed to calculate wind factor adjustment of the spread rate for. The max rates are constants derived from the litterature.
 * @return the maximal wind speed corresponding to the current cell's fuel model id.
 */
double get_max_wind_speed_for_fuel_model(map_t* map, int i, int j) {
    if (strcmp(map->map[i * map->size_of_map + j].fuel, "TL1") == 0)      return TL1_MAX_WIND_SPEED_CALC;
    else if (strcmp(map->map[i * map->size_of_map + j].fuel, "TU1") == 0) return TU1_MAX_WIND_SPEED_CALC;
    else if (strcmp(map->map[i * map->size_of_map + j].fuel, "GR1") == 0) return GR1_MAX_WIND_SPEED_CALC;
    else {
        printf("There was an error with identifying the fuel model for the cell! exiting...\n");
        exit (EXIT_FAILURE);
    }
}

/**
 * This checks the fuel model ID in current cell
 * @return wind scaling ratio for the corresponding fuel model
 */
double get_wind_scaling_for_fuel_model(map_t* map, int i, int j) {
    if (strcmp(map->map[i * map->size_of_map + j].fuel, "TL1") == 0)      return TL1_WIND_SCALING_RATIO;
    else if (strcmp(map->map[i * map->size_of_map + j].fuel, "TU1") == 0) return TU1_WIND_SCALING_RATIO;
    else if (strcmp(map->map[i * map->size_of_map + j].fuel, "GR1") == 0) return GR1_WIND_SCALING_RATIO;
    else {
        printf("There was an error with identifying the fuel model for the cell! exiting...\n");
        exit (EXIT_FAILURE);
    }
}

/**
 * Finding the topography and the elevation found in neighbor cells to find the topography
 * @return
 */
double calculate_slope_factor(map_t* map, int i, int j, direction_t neighbor_direction) {
    int neighbor_dir = neighbor_direction.direction_from_neighbor_int;
    int neighbor_index = get_neighbor_index(map, i, j, neighbor_dir);
    double elevation_of_current_cell = map->map[i * map->size_of_map + j].topography;
    double elevation_of_neighbor_cell = map->map[neighbor_index].topography;

    double distance_between_centers = CELL_WIDTH;
    if (is_diagonal(neighbor_direction.direction_from_neighbor_int) ) {//Odd direction int values are diagonals
        distance_between_centers *= SQRT_OF_2;                         //diagonals are further apart by *= sqrt(2)
    }
    double C_slope = get_slope_scaling_for_fuel_model(map, i, j);

    double delta_topography = elevation_of_current_cell - elevation_of_neighbor_cell;   //Height difference between cells (unit: m)
    double phi_slope = delta_topography / distance_between_centers; //The rise/distance [run] ratio (slope, unitless)

    // // Optional multiplier to make slope effect noticeable
    // double SLOPE_MULTIPLIER = 8;

    double slope_factor = C_slope * phi_slope;

    if (slope_factor > 5) {
        slope_factor = 5;
    }
    if (slope_factor <-1){
    slope_factor = 1;
    }

    return slope_factor;
}

/**
 * This checks the fuel ID for the current cell
 * @return slope scaling ratio for the corresponding fuel model
*/
double get_slope_scaling_for_fuel_model(map_t* map, int i, int j) {
        if (strcmp(map->map[i * map->size_of_map + j].fuel, "TL1") == 0)      return TL1_SLOPE_SCALING_RATIO;
        else if (strcmp(map->map[i * map->size_of_map + j].fuel, "TU1") == 0) return TU1_SLOPE_SCALING_RATIO;
        else if (strcmp(map->map[i * map->size_of_map + j].fuel, "GR1") == 0) return GR1_SLOPE_SCALING_RATIO;
        else {
            printf("There was an error with identifying the fuel model for the cell! exiting...\n");
            exit (EXIT_FAILURE);
        }
}

/**
 * This function checks the time spent when the program is run and updates the user
*/
void update_timekeeper(int input_time, int* all_time) {
    *all_time += input_time;
    int total = *all_time;

    int days    = total / 1440;
    total %= 1440;

    int hours   = total / 60;
    int minutes = total % 60;
    printf("\nTotal time gone by D:%d H:%d M:%d\n", days, hours, minutes);
}

/**
 *This function shows the runtime, which helps to compare
 */
double runtime_end(clock_t begin) {
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n\nThe runtime for this sim_loop iteration was: %.2lf",time_spent);
    return time_spent;
}