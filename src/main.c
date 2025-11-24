#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"
#include "simulation.h"
//definere til en konstant symbol for at sikre at der er lige mange tækker og kolonner. Så programmet ved, hvornår den skal hoppe videre til næste celle

//definiere symbolske konstanter
    //timestep til beregning
    //Base rate of spread for de 2 brænselstyper (så 2 definitioner)
    //skriv scaling factors til vind og heldning

//structtypen defineres
    //Globale parametre
        //vind hastighed
        //vind retning (nord, syd, øst, vest) -
            //Vinkelgrader - radianer, vinkel fra nord
        //fugtighed i brænsel (på sigt kunne dette være i fil. så det er pr. celle)

//Vi laver en struct som indlæser paramtrene fra filen som er hhv:
    //Topografi : elevation (double)
    //Skovtyper (en string med én type, alle samme værdier - en anden med den anden type) ID for brændseltypen
    //Status (en værdi som representere om cellen: ikke brænder,, brænder eller er udbrændt)
//map struct med size of map og arrayet indeholdt

int main() {
    //fortæl bruger at det nu er map data filen de skal skrive filstien til
    char* file_path = get_file_path_from_user(); //Bruger input for filstien til datafilen
    FILE* input_file = open_data(file_path);     //Åbn datafilen fra filstien i read mode
    array_t array;                               //Erklær en variabel struct af datastrukturen
    get_size_of_map(input_file, &array);         //Opdater array.size_of_map fra datafilens værdi for størrelsen
    initialize_array(&array);                    //Allokerer arrayet dynamisk, skal bruge size_of_map
    get_data_from_file(input_file, &array);      //Initialiserer celler i map til værdierne fra datafilen
    print_grid(&array);

    global_params_t global_params;
    //fortæl bruger at det nu er globale parametre filen de skal skrive filstien til
    //sim_loop(&array, &global_params);          //Kører simulationsdelen af programmet, som i et loop tager input for
                                                 //simulationsvarighed, simulerer spredning af ild
                                                 //og printer mappet af cellers status (brand eller ingen brand)
    free_memory(&array);
    return EXIT_SUCCESS;
    //sidekommentar:
    //kortfilden
    //ilden starter altid fra centrum af kort - spørger ikke brugeren men sættes som standard
}