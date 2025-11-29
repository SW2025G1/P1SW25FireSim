#include <math.h>
#include "functions.h"
FILE* get_file_path_from_user() {
    char filsti[256]; //array hvor man skriver filnavnet
    FILE *fptr; // File* bliver lavet her, så den kan være med i do while loopet.
    do {
        printf("Enter absolute or relative filepath to selected map scenario\n");
        printf("Suggested input:   src/Data_txt   \n>");
        scanf(" %255s", filsti);

        fptr = fopen(filsti, "r");
        //tilføjer tjek for, at se om filen åbnes korrekt
        if (fptr == NULL) {
            printf("The filepath %s did not result in succesful file open\n", filsti);
        }
    } while (fptr == NULL);
    return fptr;
}


void get_size_of_map(FILE *fptr, map_t* map) {
    fscanf(fptr, "%d", &map->size_of_map);
    if (map->size_of_map < 1 || map->size_of_map > MAX_MAP_SIZE) {
        printf("Error\n");
        exit(EXIT_FAILURE);
    }
    printf("Size of map was assigned to grid of:%d x %d cells\n",map->size_of_map, map->size_of_map);
    // Lav en funktion der læser size of map fra data filen
    //Initialisere vores array ved brug af struct. Vi sætter SIZE_OF_MAP * SIZE_OF_MAP for, at få 2D array grid
    // alle starter med status 0 på alle celler ud over den der starter med at brænde, denne starter med værdien 1
    //cell_t map[SIZE_OF_MAP][SIZE_OF_MAP];
}
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
 * @param map
 * @param array Datapakken der indeholder size_of_map og selve griddet (array af cell_t structs)
 */
void initialize_map(map_t* map) {
    map->map = initialize_map_helper(map->map, map);
    map->temp_map = initialize_map_helper(map->temp_map, map);
}
void initial_burning_cell(map_t* map) {
    int i = map->size_of_map / 2;
    int j = i;
    map->map[i * map->size_of_map + j].status = 1.0;
}

void get_data_from_file(FILE *fptr, map_t* map) {
    //tag noget data ind for kortet/det geografiske område  - topografien (linear binary search)
    //1. åbne fil (filnavn) indholdene: tekstfil, med datafelter for hver celle
    //"r" betyder at den læser filen i read-mode og derfor ikke ændre i den
    //2. for loop
    //tager det antal data felter der skal være i sctructen og læser ind (fscanf?) på de rigtige felter i cellens struct
    //3. Gentages for alle cellerne og loopet slutter - filen lukkes
    //indlæs size of map fra starten af datafilen til en variabel
    for (int i = 0; i < map->size_of_map; i++) {
        for (int j = 0; j < map->size_of_map; j++) {
            fscanf(fptr, " %lf", &map->map[i * map->size_of_map + j].topography);
            fscanf(fptr, " %3s",  map->map[i * map->size_of_map + j].fuel);
            fscanf(fptr, " %lf", &map->map[i * map->size_of_map + j].status);
        }
    }
    fclose(fptr);
    initial_burning_cell(map);
}
void print_grid(map_t* map){
    //debug_print(map); //debug printet bruges først hvis vi skal lave om i get_data funktionen og har brug for at vide om det virker.

    //Funktionen print_kort(size_of_grid, struct* array)
    // size of grid skal kalde en anden funktion, hvor grid størrelsen ligger i
    //1. for loop (ydre) til at printe linjerne (den bliver ved med at printe til der ikke er flere rækker (size_of_grid))
    //indre loop: for hver række skal den printe repræsentation af hver celle-status
    //2. status for hvor lang tid der er gået

    for (int i = 0; i < map->size_of_map; i++) {
        for (int j = 0; j < map->size_of_map; j++) {
            if (map->map[i * map->size_of_map + j].status < 1) {
                printf(".  ");
            }
            else if (map->map[i * map->size_of_map + j].status > 0 && map->map[i * map->size_of_map + j].status < 1) {
                printf("~  ");
            }
            else {
                printf("F  ");
            }
        }
        printf("\n");
    }
}

void free_memory(map_t* map) {
    printf("Freeing memory\n");
    free(map->map);
}
// Implementation of example function


//tage imod vind, vejr, .. fra bruger - printf, scanf
Weather_t weather_input_from_user() {
    Weather_t w;

    printf("\nEnter scenario conditions\n\n");
    printf("The moisture level of dead surface fuel (0.10 = 10 percent moisture): \n");
    printf("Suggested inputs:   0.05 (drought period)  0.10 (dry)   0.15   (regularly observed value)   0.30 (moist)\n>");
    scanf(" %lf", &w.moisture_of_fuel);
    printf("Wind speed (m/s):\n> ");
    scanf(" %lf", &w.wind_speed);src
    printf("Wind directions: 0 (E)  1 (NE)  2 (N)  3 (NW)  4 (W)  5 (SW)  6 (S)  7 (SE)\n>");
    scanf(" %d", &w.wind_direction);

    printf("\nThe entered weather conditions are as follows\n");
    printf("Dead fuel moisture: %.2lf\n", w.moisture_of_fuel);
    printf("Wind speed: %.2lf\n", w.wind_speed);
    printf("Wind directions: 0 (E)  1 (NE)  2 (N)  3 (NW)  4 (W)  5 (SW)  6 (S)  7 (SE)\n");
    printf("Wind direction: %d\n", w.wind_direction);

    w.wind_direction_radians = w.wind_direction * (M_PI / 4); //These enum types match the actual radians conversions
    //by this operation. We will need the direction in radians for some calculations.
        return w;
    }

    void debug_print(map_t* map) {
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
