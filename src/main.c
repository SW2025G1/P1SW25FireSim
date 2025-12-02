#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "functions.h"
#include "simulation.h"


int main() {
    enable_ansi_codes(); //For enabling background color of character field in the output terminal to output visual representation of cells in the program

    FILE* file = get_file_path_from_user(); //Bruger input for filstien til datafilen og åbner den i read mode

    map_t map;                               //Erklær en variabel struct af datastrukturen

    get_size_of_map(file, &map);         //Opdater array.size_of_map fra datafilens værdi for størrelsen

    Weather_t w = weather_input_from_user();  //Spørger brugeren efter vejrforhold - temperatur, vindhastighed, vindretning og luftfugtighed.

    initialize_map(&map);                    //Allokerer arrayet dynamisk, skal bruge size_of_map

    get_data_from_file(file, &map);      //Initialiserer celler i map til værdierne fra datafilen

    print_grid(&map);

    sim_loop(&map, &w);             //Kører simulationsdelen af programmet, som i et loop tager input for
                                    //simulationsvarighed, simulerer spredning af ild
                                    //og printer mappet af cellers status (brand eller ingen brand)
    free_memory(&map);
    return EXIT_SUCCESS;

}

