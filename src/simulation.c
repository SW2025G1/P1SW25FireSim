#include "simulation.h"
#include <math.h>
#include <string.h>

#include "functions.h"

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
    const char *OUTPUT_DIR = "output";

    do {
        input_time_or_exit(&input_time);

        if (input_time != 0) {
            for (int k = TIME_STEP; k < input_time; k += TIME_STEP) {
                for (int i = 1; i < map->size_of_map - 1; i++) { //i is set to 1, to ensure that it skips the first column
                    for (int j = 1; j < map->size_of_map - 1; j++) {
                        calculate_new_status(map, w, i, j);
                    }
                }


                //Copy all bytes from map->temp_map data area to map->map data area.
                memcpy(map->map, map->temp_map, map_size_bytes);
            }
        }
        print_grid(map);
        update_timekeeper(input_time, &all_time);
        simulation_run_count++;

        if (map->size_of_map >= 36) {
            // Generate the file name and path
            char filename[256];
            char command[512];

            // NEW FILNAMELOGIC: frame_[antal_kørsler]_[samlet_tid_minutter].html
            sprintf(filename, "%s/frame_%04d_T%dmin.html",
                    OUTPUT_DIR,
                    simulation_run_count,
                    all_time);

            write_map_to_html(map, filename);

            char *open_command = NULL;

            #if defined(_WIN32) || defined(_WIN64)
                        open_command = "start";
            #elif defined(__APPLE__)
                        open_command = "open";
            #elif defined(__linux__)
                        open_command = "xdg-open";
            #endif

            if (open_command) {
                sprintf(command, "%s %s", open_command, filename);
                if (system(command) != 0) {
                    printf("Warning, could not open syscommand. Open %s manually from output dir.\n", filename);
                }
            }
            printf("Time run: %d minutes (Since initial ignition: %d minutes)\n",
                   input_time, all_time);
        }

    } while (input_time != 0);
}

/**
 * This function is a helper to the simulation loop, where this adds the possibility for the user to decided on the amount time they wish for the simulation to run
*/
void input_time_or_exit(int* input_time) {
    do {
        printf("Welcome to the simulation. You are to make inputs for how long you wish the simulation run.\n"
          "Input 0: Exit the program.\n"
          "Input 1: Run simulation for 10 minutes\n"
          "Input 2: Run simulation for 30 minutes\n"
          "Input 3: Run simulation for 60 minutes\n"
          "Input 4: Run simulation for 3 hours\n"
          "Input 5: Run simulation for 12 hours\n"
          "Input 6: Run simulation for 24 hours\n"
          );
        scanf(" %d", input_time);
        printf("entered input= %d\n"
               "Running sims\n\n",*input_time);

        if (*input_time < 0 || *input_time > 6) {
            printf("Wrong. (0-6 please).\n");
        }
    } while (*input_time < 0 || *input_time > 6);

    *input_time = convert_input_to_time(input_time);
}

/**
 * This converts the decided upon time picked by the user in the funtion above to a specific time we can then use to calculate
*/
int convert_input_to_time(int* input_time) {
    switch (*input_time) {
        case 1: return 10;
        case 2: return 30;
        case 3: return 60;
        case 4: return 180;
        case 5: return 720;
        case 6: return 1440;
        default: return 0;
    }
}

/**
 * This calculates the affect each neighbor has to the cell currently being calculated
 */
void calculate_new_status(map_t* map, Weather_t* w, int i, int j) {
    for (int direction = East; direction < DIRECTIONS_AMOUNT; direction ++) { //Angle we calculate is 0 as the starting point - we then ad one fourth to each time (thereby starting in: direction_from_neighbor: East)
        direction_t neighbor_direction;
        neighbor_direction.direction_from_neighbor_int = direction;     //The enum type corresponds to the integer values 0-7 from 0: East to 7: SouthEast
        neighbor_direction.direction_from_neighbor_radians = direction * (M_PI / 4); //These enum types match the actual radians conversions by this operation
        map->temp_map[i * map->size_of_map + j].status += status_calculator(map, w, i, j, neighbor_direction);
    }
}

/**
 *
 * @param map
 * @param w
 * @param i
 * @param j
 * @param neighbor_direction
 * @return
 */
double status_calculator(map_t* map, Weather_t* w, int i, int j, direction_t neighbor_direction) {
    int neighbor_dir = neighbor_direction.direction_from_neighbor_int;
    int neighbor_index = get_neighbor_index(map, i, j, neighbor_dir);
    double status_update = 0;

    //if the neighbor cell to calculate spread from is not yet burning, no calculations should be done.
    if (map->map[neighbor_index].status >= 1.0) {
        double distance_between_centers = CELL_WIDTH;
        if (neighbor_direction.direction_from_neighbor_int % 2 == 1) {//TODO: this should be a function call, as it is reused in slope function
            distance_between_centers *= SQRT_OF_2;    //If the direction integer value is odd, it must be one of the diagonals. Therefore
            //The actual distance from center of cell to center of cell is larger by the square root of two ratio
        }

        double base_rate_of_spread = calculate_base_rate(map, w, i, j, neighbor_direction); //TODO: indsæt if statement //Hvis 0 så lad værd med at regne de næste
        double wind_factor = calculate_wind_factor(map, i, j, w, neighbor_direction); //denne wind_direction skal laves om til double - se kommentar i simulation.h
        double slope_factor = calculate_slope_factor(map, i, j, neighbor_direction); //elevation fra sig selv og fra k-retning
        double total_spread_rate = calculate_total_spread_rate(base_rate_of_spread, wind_factor, slope_factor); //funktion tager foregående calculations og samler
        double ignition_time = distance_between_centers / total_spread_rate; //distance pr. rate - fx meter pr. min.

        status_update = TIME_STEP / ignition_time;//Status is the value that tells whether the cell should be ignited in the timestep
        return status_update;     //which accumulates in the cell value over time steps and between directions of spread to the cell calculated in the same time step
    }
    else {
        return 0.0;
    }
}

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

double calculate_base_rate(map_t* map, Weather_t* w, int i, int j, direction_t direction_from_neighbor) {
    //tage base rate - funktionen skal bruge fuel model id og moisture fra weather
    double extinction_moisture_of_cell; //set to safe value in case assigning values fails in below.
    double base_base_rate; //The base rate not modified for moisture
    update_base_rate_values(map, &base_base_rate, &extinction_moisture_of_cell, i, j);

    return base_base_rate * (1 - (w->moisture_of_fuel / extinction_moisture_of_cell));
}

double calculate_total_spread_rate(double base_rate_of_spread, double wind_factor, double slope_factor) {
    return base_rate_of_spread * (1 + wind_factor + slope_factor);
}

void update_base_rate_values(map_t* map, double* base_base_rate, double* extinction_moisture_of_cell, int i, int j) {
    if (strcmp(map->map[i * map->size_of_map + j].fuel, "TL1") == 0) {
        *extinction_moisture_of_cell = TL1_MOISTURE_EXTINCTION;
        *base_base_rate = TL1_BASE_BASE_RATE;

    } else if (strcmp(map->map[i * map->size_of_map + j].fuel, "TU1") == 0) {
        *extinction_moisture_of_cell = TU1_MOISTURE_EXTINCTION;
        *base_base_rate = TU1_BASE_BASE_RATE;
    }
    else {
        printf("There was an error with identifying the fuel model for the cell! exiting...\n");
        exit (EXIT_FAILURE);
    }
}

double calculate_wind_factor(map_t* map, int i, int j, Weather_t* w, direction_t neighbor_direction) {
    double C_wind = get_wind_scaling_for_fuel_model(map, i, j);

    return fmax(0, C_wind * w->wind_speed * cos(w->wind_direction_radians - neighbor_direction.direction_from_neighbor_radians) );

    //hvor meget bidrager vinden til at den spreder sig hurtigerre i den angivne retning
    //k = retning - vi vil gerne beregne for denne
    //den faktiske vindretning
    //hvis den er negativ, dvs. vind ikke blæser i den retning = 0
    //hastighed som er komponenten af vinden - matematisk funktion - max(0,f(kom)) <- tager maksimale værdi mellem 2. Hvis den højre er positiv, så vælger den den. Hvis den er negativ, så vælger den 0

    //R = R_0 * (1 + theta_wind_1 i + slope factor_1 y)
    //return max(0,f(kom)); //f(kom) er beregningsresultatet - theta wind (wind factor)
}
double get_wind_scaling_for_fuel_model(map_t* map, int i, int j) {
    if (strcmp(map->map[i * map->size_of_map + j].fuel, "TL1") == 0) {
        return TL1_WIND_SCALING_RATIO;
    } else if (strcmp(map->map[i * map->size_of_map + j].fuel, "TU1") == 0) {
        return TU1_WIND_SCALING_RATIO;
    }
    else {
        printf("There was an error with identifying the fuel model for the cell! exiting...\n");
        exit (EXIT_FAILURE);
    }
}

double calculate_slope_factor(map_t* map, int i, int j, direction_t neighbor_direction) {
    double elevation_of_current_cell = map->map[i * map->size_of_map + j].topography;
    int neighbor_dir = neighbor_direction.direction_from_neighbor_int;
    int neighbor_index = get_neighbor_index(map, i, j, neighbor_dir);
    double elevation_of_neighbor_cell = map->map[neighbor_index].topography;
    double distance_between_centers = CELL_WIDTH;
    if (neighbor_direction.direction_from_neighbor_int % 2 == 1) {
        distance_between_centers *= SQRT_OF_2;    //If the direction integer value is odd, it must be one of the diagonals. Therefore
        //The actual distance from center of cell to center of cell is larger by the square root of two ratio
    }
    double C_slope = get_slope_scaling_for_fuel_model(map, i, j);

    double delta_topography = elevation_of_current_cell - elevation_of_neighbor_cell;   //Height difference between cells (unit: m)
    double phi_slope = delta_topography / distance_between_centers; //The rise/distance [run] ratio (slope, unitless)

    return  fmax(0, C_slope * phi_slope);
}

double get_slope_scaling_for_fuel_model(map_t* map, int i, int j) {
        if (strcmp(map->map[i * map->size_of_map + j].fuel, "TL1") == 0) {
            return TL1_SLOPE_SCALING_RATIO;
        } else if (strcmp(map->map[i * map->size_of_map + j].fuel, "TU1") == 0) {
            return TU1_SLOPE_SCALING_RATIO;
        }
        else {
            printf("There was an error with identifying the fuel model for the cell! exiting...\n");
            exit (EXIT_FAILURE);
        }
}

void update_timekeeper(int input_time, int* all_time) {
    *all_time += input_time;
    int total = *all_time;

    int days    = total / 1440;
    total %= 1440;

    int hours   = total / 60;
    int minutes = total % 60;
    printf("total time gone by D:%d H:%d M:%d \n", days, hours, minutes);
}