#include <math.h>
#include "input-output.h"
#include "simulation.h"
#include "constants-and-enum.h"
/**
 * @return the file entered by the user or print error if the file is not successfully reached
 */
FILE* get_file_path_from_user(void) {
    char filsti[256];
    FILE *fptr;
    do {
        printf("Enter absolute or relative filepath to selected map scenario\n");
        printf("For input file names: please see maps/ folder in the file dir\n"
               "Example input: maps/TL1_300x300.txt\n\n>");
        scanf("%255s", filsti);

        fptr = fopen(filsti, "r");
        //Adding af check to see if the files is opening up correctly
        if (fptr == NULL) {
            printf("The filepath %s did not result in successful file open\n", filsti);
        }
    } while (fptr == NULL);
    return fptr;
}

/**
 * @param fptr  is used to read the top line only in the dataset to read the size of map
 * @param map we then use the struct to initialize and get a 2D print of the grid.
 */

void get_size_of_map(FILE *fptr, map_t* map) {
    fscanf(fptr, "%d", &map->size_of_map);
    if (map->size_of_map < 1 || map->size_of_map > MAX_MAP_SIZE) {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    printf("Size of map was assigned to grid of:%d x %d cells\n"
           "This corresponds to an area of %d x %d meters",map->size_of_map, map->size_of_map,
           CELL_WIDTH * map->size_of_map, CELL_WIDTH * map->size_of_map);
}

/**
 * A support function to allocate the memory of the decided upon size of map. Used twice (map->map and map->temp_map)
*/
cell_t* initialize_map_helper (cell_t* map_ptr, map_t* map) {
    map_ptr = malloc((size_t)map->size_of_map * map->size_of_map * sizeof(cell_t));
    if (map_ptr == NULL) {
        printf("Memory error with allocating! exiting now :(");
        exit (EXIT_FAILURE);
    }
    return map_ptr;
}

/**
 * @brief initialize_array skal dynmaisk allokere hukommelse til arrayet af cell_t structs, på pladsen, hvor cell_t pointeren "map" peger på (i datastructet).
 * @param map array Datapakken der indeholder size_of_map og selve griddet (array af cell_t structs)
 */
void initialize_map(map_t* map) {
    map->map = initialize_map_helper(map->map, map);
    map->temp_map = initialize_map_helper(map->temp_map, map);
}

/**
 * @brief This function 'ignites' the center cell of the map
 * @param map The data struct map is received to find and manipulate the center cell.
 */
void initial_burning_cell(map_t* map) {
    int i = map->size_of_map / 2;
    int j = i;
    map->map[i * map->size_of_map + j].status = 1.0;
}

/**
 * importing data from the datafile to their designated places in the struct in each cell
 * @param fptr used to read from file
 * @param map is the struct in which the data is file scanned to.
 * End by initial_burning_cell to ignite the center cell
 */
void get_data_from_file(FILE *fptr, map_t* map) {
    for (int i = 0; i < map->size_of_map; i++) {
        for (int j = 0; j < map->size_of_map; j++) {
            fscanf(fptr, " %lf %3s %lf",
                &map->map[i * map->size_of_map + j].topography,
                map->map[i * map->size_of_map + j].fuel,
                &map->map[i * map->size_of_map + j].status);
        }
    }
    fclose(fptr);
    initial_burning_cell(map);
}

/**
 * This function is used to print out the grid containing colors that symbolise if the cell is burnt (red), burning (yellow) or not burning (green)
 * @param map Is here since we have to use size_of_map to get the size from the dataset found in the function get_size_of_map
 */

void print_grid(map_t* map) {
    printf("\n");

    if (map->size_of_map > 36) {
        printf("Map size extends beyond the terminal capabilities. Will not print in terminal. \n"
               "You can see the output in the html file in the /output folder.\n\n Continuing simulation...\n\n");
        return;
    }

    int mid = map->size_of_map / 2;

    for (int i = 0; i < map->size_of_map; i++) {
        for (int j = 0; j < map->size_of_map; j++) {

            double status = map->map[i * map->size_of_map + j].status;

            const char *bg;
            if (status >= 1) {
                bg = RED_BG;
            } else if (status > 0 && status < 1) {
                bg = YELLOW_BG;
            } else {
                bg = GREEN_BG;
            }

            if (i == mid && j == mid) {
                printf("%s X %s", bg, RESET);
            } else {
                printf("%s   %s", bg, RESET);
            }
        }
        printf("\n");
        fflush(stdout);
    }
}

/**
 * @brief This function prints a progress bar in the terminal, so the user knows the simulation is running, even when runtime is long.
 * @param k The current time step in the sim_loop. Needed to calculate the progress for the print
 * @param input_time The user input time to simulate in sim_loop. Used to calculate the progress and times to run the sim_loop loop
 */
void print_progress(double k,  int input_time) {
    int times_to_run = (int)(input_time / TIME_STEP);

    double progress_percent = (k / input_time) * 100.0;
    int fill_count = (int)(progress_percent * BAR_WIDTH / 100.0);

    //Move the cursor to the start of the line to overwrite previous progress bar print.
    printf("\r[");
    //Print filled part of progress bar
    for (int i = 0; i < fill_count; ++i) printf("#");
    //Print empty part of progress bar
    for (int j = 0; j < BAR_WIDTH - fill_count; ++j) printf(" ");
    //Print end of progress bar message with info
    printf("] %4.2lf%% of time steps run. Total steps = %d",progress_percent, times_to_run);
    fflush(stdout); // Print immediately
}

/**
 * Via input from the user, this function takes the values: moisture, wind_speed and wind_direction
 * @return w which is the struct Weather
 */
Weather_t weather_input_from_user(void) {
    Weather_t w;

    printf("\nEnter scenario conditions\n\n");
    printf("The moisture level of dead surface fuel (0.10 = 10 percent moisture): \n");
    printf("Suggested inputs:   0.05 (drought period)  0.10 (dry)   0.15   (regularly observed value)   0.30 (moist)\n>");
    scanf(" %lf", &w.moisture_of_fuel);
    printf("Wind speed (m/s):\n> ");
    scanf(" %lf", &w.wind_speed);
    printf("Wind directions: 0 (E)  1 (NE)  2 (N)  3 (NW)  4 (W)  5 (SW)  6 (S)  7 (SE)\n>");
    scanf(" %d", &w.wind_direction);

    printf("\nThe entered weather conditions are as follows\n");
    printf("Dead fuel moisture: %.2lf\n", w.moisture_of_fuel);
    printf("Wind speed: %.2lf\n", w.wind_speed);
    printf("Wind directions: 0 (E)  1 (NE)  2 (N)  3 (NW)  4 (W)  5 (SW)  6 (S)  7 (SE)\n");
    printf("Wind direction: %d\n", w.wind_direction);

    w.wind_direction_radians = w.wind_direction * (M_PI / 4); //These enum types match the actual radians conversions
        return w;
    }

/**
 * This function is a helper to the simulation loop, where this adds the possibility for the user to decided on the amount time they wish for the simulation to run
*/
void input_time_or_exit(int* input_time) {
    do {
        printf("\nSim loop start. You are to make inputs for how long you wish the simulation run.\n"
          "Input 0: Exit the program.\n"
          "Input 1: Run simulation for 10 minutes\n"
          "Input 2: Run simulation for 30 minutes\n"
          "Input 3: Run simulation for 60 minutes\n"
          "Input 4: Run simulation for 3 hours\n"
          "Input 5: Run simulation for 10 hours\n"
          "Input 6: Run simulation for 12 hours\n"
          "Input 7: Run simulation for 24 hours\n"
          );
        scanf(" %d", input_time);
        printf("entered input= %d. Running sims",*input_time);

        if (*input_time < 0 || *input_time > 7) {
            printf("Wrong. (0-7 please).\n");
        }
    } while (*input_time < 0 || *input_time > 7);

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
        case 5: return 600;
        case 6: return 720;
        case 7: return 1440;
        default: return 0;
    }
}
/**
 * This function is made as a debugging tool for the function: print_grid
 * @param map
 */
void debug_print(map_t* map) { //TODO: perhaps separate debugging and the enable ansi codes into a clutter file? So the simulation logic is not confused by these obscure functions
    for (int i = 0; i < map->size_of_map; i++) {
        printf("\n");
        for (int j = 0; j < map->size_of_map; j++) {
            printf("Reached this point\n");
            printf("Cell [%d][%d] has values: topo: %.2lf, fuel: %s, status: %.2lf \n", i, j,
            map->map[i * map->size_of_map + j].topography,
            map->map[i * map->size_of_map + j].fuel,
            map->map[i * map->size_of_map + j].status);
        }
    }
}
/**
 * Here we free the memory that where allocated with the initialize_map function
 * @param map
 */
void free_memory(map_t* map) {
    if (map == NULL) return;

    printf("Freeing memory\n");
    free(map->map);
    map->map = NULL;

    free(map->temp_map);
    map->temp_map = NULL;

    free(map);
}
/**
 * This function activates ANSI-codes on Windows consoles. These are used for the color setting of the character blocks in the print_grid function.
 */
void enable_ansi_codes(void) {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    // ENABLE_VIRTUAL_TERMINAL_PROCESSING is the flag that enables ANSI codes to function
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}