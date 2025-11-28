#include "simulation.h"
#include <math.h>
#include <string.h>

#include "functions.h"

#define TIME_STEP 5
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

void sim_loop(map_t* map, Weather_t* w) {
    char input_char = 'y';
    int input_time = 0;

    map_t temp_map; //We need a temporary grid to store the status updates in to prevent calculating with next time step values within the timestep when iterating neighboring cell to next time step ignited cells.
    initialize_map(&temp_map);
    do {
        input_time_or_exit(&input_char, &input_time);

        if (input_time != 0) { //if still 0, the user exited the program.
            for (int k = TIME_STEP; k < input_time; k += TIME_STEP) {
                for (int i = 1; i < map->size_of_map - 1; i++) { //i sættes til 1, fordi den springer over første kolonne
                    for (int j = 1; j < map->size_of_map - 1; j++) {
                        calculate_new_status(map, &temp_map, w, i, j);
                    }
                }
                memcpy(map, &temp_map, sizeof(*map)); //Copies the temporary map with new status values into actual map for each time step.
            }
        }
        print_grid(map);
    } while (input_time != 0);

    free_memory(&temp_map);
   //ind i simulationsloopet - køres (ydre loop)
    //Brugeren bestemmer, hvor lang simulationen skal kører /time, dage, ??? (starter med én fast tid/valgmuligehed (1 time))

    //Det indre loop
    //Udregning pr. celle om cellen er gået i brand - ny status
    //akummulere en status for hver beregning cyklus - skal bygge op til ny antendelse i næste kørsel af loop
    //skal opdatere med naboernes spredningsratio i visse timesteps
    //skal opdatere med mindre interval end brugeren har inputtet - da brænden speder sig hurtigere end én celle pr. time osv.
    //vi sætter fast værdi (senere opdatering: skal også her opdatere timesteppets størrelse)

    //Vi er ude i det ydre loop igen - Print grid + spørger brugeren om de vil fortsætte eller exit
}

void input_time_or_exit(char* input_char, int* input_time) {
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
    } while (*input_time < 0 || *input_time > 6);
  }

void calculate_new_status(map_t* map, map_t* temp_map, Weather_t* w, int i, int j) {
    //Fokuserer på én celle
    //8 ting
    //loop - kalder funktion der beregner fra specifik retning
    //loop starter fra sydcelle
    //sydrate
    for (int direction = East; direction < DIRECTIONS_AMOUNT; direction ++) { //vinkel vi beregner på er 0 til start - lægger pi fjerdedel til pr gang (starter altså med direction_from_neighbor: East)
        direction_t neighbor_direction;
        neighbor_direction.direction_from_neighbor_int = direction;     //The enum type corresponds to the integer values 0-7 from 0: East to 7: SouthEast
        neighbor_direction.direction_from_neighbor_radians = direction * (M_PI / 4); //These enum types match the actual radians conversions by this operation
        temp_map->map[i * map->size_of_map + j].status += status_calculator(map, w, i, j, neighbor_direction); //nabocellernes bidrag til at tillægge statusværdi til cellen i temp_map
    }
}

double status_calculator(map_t* map, Weather_t* w, int i, int j, direction_t neighbor_direction) {
    double status_update = 0;

    double base_rate_of_spread = calculate_base_rate(map, w, i, j, neighbor_direction);
    double wind_factor = calculate_wind_factor(map, i, j, w, neighbor_direction); //denne wind_direction skal laves om til double - se kommentar i simulation.h
    double slope_factor = calculate_slope_factor(map, i, j, neighbor_direction); //elevation fra sig selv og fra k-retning
    double total_spread_rate = calculate_total_spread_rate(base_rate_of_spread, wind_factor, slope_factor); //funktion tager foregående calculations og samler
    double ignition_time = CELL_WIDTH / total_spread_rate; //distance pr. rate - fx meter pr. min.

    status_update = TIME_STEP / ignition_time;//Status is the value that tells whether the cell should be ignited in the timestep
    return status_update;     //which accumulates in the cell value over time steps and between directions of spread to the cell calculated in the same time step

}

double calculate_base_rate(map_t* map, Weather_t* w, int i, int j, direction_t direction_from_neighbor) {
    //tage base rate - funktionen skal bruge fuel model id og moisture fra weather
    double extinction_moisture_of_cell = 1.00; //set to safe value in case assigning values fails in below.
    double base_base_rate = 0.00; //The base rate not modified for moisture
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

    return C_wind * w->wind_speed * fmax(0, cos(w->wind_direction_radians - neighbor_direction.direction_from_neighbor_radians));

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

double calculate_slope_factor(map_t* map, int i, int j, direction_t neighbor_direction) { //TODO: Fix slope factor
    double elevation_of_current_cell = map[i * map->size_of_map + j].map->topography;
    double elevation_of_neighbor_cell = get_neighbour_elevation(map, i, j, neighbor_direction);
    if (elevation_of_neighbor_cell >= elevation_of_current_cell) {
        return 0.0;//If we are calculating a downslope, the slope will NOT contribute positively to fire spread.
    }//We are enforcing non negativity of slope, and the wind, factor, to avoid the potential of negative total spread rate
    double distance_between_centers = CELL_WIDTH;
    if (neighbor_direction.direction_from_neighbor_int % 2 == 1) {
        distance_between_centers *= SQRT_OF_2;    //If the direction integer value is odd, it must be one of the diagonals. Therefore
        //The actual distance from center of cell to center of cell is larger by the square root of two ratio
    }
    double C_slope = get_slope_scaling_for_fuel_model(map, i, j);

    double delta_topography = elevation_of_current_cell - elevation_of_neighbor_cell;   //Height difference between cells (unit: m)
    double phi_slope = delta_topography / distance_between_centers; //The rise/distance ratio (slope, unitless)

    return  C_slope * phi_slope;

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

double get_neighbour_elevation(map_t* map, int i, int j, direction_t neighbor_direction) {//TODO: Fix this
    switch(neighbor_direction.direction_from_neighbor_int) {
        case East:      return map[i * map->size_of_map + (j + 1)].map->topography;
        case NorthEast: return map[(i - 1) * map->size_of_map + (j + 1)].map->topography;
        case North:     return map[(i - 1) * map->size_of_map + j].map->topography;
        case NorthWest: return map[(i - 1) * map->size_of_map + (j - 1)].map->topography;
        case West:      return map[i * map->size_of_map + (j - 1)].map->topography;
        case SouthWest: return map[(i + 1) * map->size_of_map + (j - 1)].map->topography;
        case South:     return map[(i + 1) * map->size_of_map + j].map->topography;
        case SouthEast: return map[(i + 1) * map->size_of_map + (j + 1)].map->topography;

        default: {
            printf("Error getting topography of neighbor cell. Exiting!\n");
            exit(EXIT_FAILURE);
        }
    }
}

