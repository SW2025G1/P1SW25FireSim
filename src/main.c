#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "functions.h"
#include "simulation.h"
#include "visualizer.h"


int main() {
    enable_ansi_codes(); //For enabling background color of character field in the output terminal to output visual representation of cells in the program

    FILE* file = get_file_path_from_user(); //Opens up data files in read mode

    map_t map;                               //A declaration of a variable to a struct of the data structure

    get_size_of_map(file, &map);         //Updating array.size_of_map from the value of the datafile for the given size

    Weather_t w = weather_input_from_user();  //Asking for weather input, temperature, wind speed, wind direction and air humidity.

    initialize_map(&map);                    //Allocating the array dynamic

    get_data_from_file(file, &map);      //Initializing the cells in the map for the values from the data file.

    print_grid(&map);

    sim_loop(&map, &w);             //Running the simulation part of the program, as a loop containing:
                                    //the duration of the simulation, how the spread of fire moves and prints the map of the cells status (fire or no fire)
    free_memory(&map);
    return EXIT_SUCCESS;

}

