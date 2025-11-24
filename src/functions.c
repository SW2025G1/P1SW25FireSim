#include <math.h>
#include "functions.h"


FILE* open_data(char* file_name) {
    FILE *fptr;
    fptr = fopen("src/data_txt", "r");
    //tilføjer tjek for, at se om filen åbnes korrekt
    if (fptr == NULL) {
        printf("Filen blev ikke åbnet korrekt\n");
        exit(EXIT_FAILURE);
    }
    return fptr;
}


//TODO: Implement user data input of relative file path (src/"..."):
char* get_file_path_from_user() {
    char filsti[256]; //array hvor man skriver filnavnet
    FILE *fptr; // File* bliver lavet her, så den kan være med i do while loopet.
    do {
        printf("Indtast filsti til map:");
        scanf("%255s", filsti);

        fptr = fopen(filsti, "r");
        if (fptr == NULL) {
            printf("Kunne ikke åbne filen: %s\n", filsti);
        }
    }
    while (fptr == NULL);
}


int get_size_of_map(FILE *fptr){
    int size;
    if (fscanf(fptr, "%d", &size) != 1) {
        printf("Error\n");

        exit(EXIT_FAILURE);

    }
    printf("Size of map:%dx%d",size, size);
    return size;
    // Lav en funktion der læser size of map fra data filen
    //Initialisere vores array ved brug af struct. Vi sætter SIZE_OF_MAP * SIZE_OF_MAP for, at få 2D array grid
    // alle starter med status 0 på alle celler ud over den der starter med at brænde, denne starter med værdien 1
    //cell_t map[SIZE_OF_MAP][SIZE_OF_MAP];
}

/**
 * @brief initialize_array skal dynmaisk allokere hukommelse til arrayet af cell_t structs, på pladsen, hvor cell_t pointeren "map" peger på (i datastructet).
 * @param array Datapakken der indeholder size_of_map og selve griddet (array af cell_t structs)
 */
void initialize_array(array_t* array) {
    array->map = malloc(sizeof(cell_t) * (array->size_of_array * array->size_of_array));
    if (array->map == NULL) {
        printf("Memory error with allocating! exiting now :(");
        exit (EXIT_FAILURE);
    }
}
FILE* get_data_from_file(FILE *fptr, cell_t *array, int size_of_map) {
    //tag noget data ind for kortet/det geografiske område  - topografien (linear binary search)
    //1. åbne fil (filnavn) indholdene: tekstfil, med datafelter for hver celle
    //"r" betyder at den læser filen i read-mode og derfor ikke ændre i den
    //2. for loop
    //tager det antal data felter der skal være i sctructen og læser ind (fscanf?) på de rigtige felter i cellens struct
    //3. Gentages for alle cellerne og loopet slutter - filen lukkes
    //indlæs size of map fra starten af datafilen til en variabel
    fscanf(fptr, "%*[^\n]\n");
    for (int i = 0; i < size_of_map; i++) {
        for (int j = 0; j < size_of_map; j++) {
            fscanf(fptr, "%lf", &array[i * size_of_map + j].topography);
            fscanf(fptr, "%3s",  array[i * size_of_map + j].fuel);
            fscanf(fptr, "%lf", &array[i * size_of_map + j].status);

        }

        fclose(fptr);
    }
}
void print_grid(array_t* array){

    //Funktionen print_kort(size_of_grid, struct* array)
    // size of grid skal kalde en anden funktion, hvor grid størrelsen ligger i
    //1. for loop (ydre) til at printe linjerne (den bliver ved med at printe til der ikke er flere rækker (size_of_grid))
    //indre loop: for hver række skal den printe repræsentation af hver celle-status
    //2. status for hvor lang tid der er gået

    for (int i = 0; i < array->size_of_array; i++) {
        for (int j = 0; array->size_of_array; j++) {
            if (array->map[i * array->size_of_array + j].status < 1) {
                printf(".  ");
            } else if (array->map[i * array->size_of_array + j].status > 3){
                printf("B  ");
            }else {
                printf("F ");
            }
        }
        printf("\n");
    }

    //Denne del af koden beholdes til de-buggin print
    for (int i = 0; i < size_of_map; i++) {
        for (int j = 0; j < size_of_map; j++) {
            printf("Celle [%d][%d] has values: topo: %.2lf, fuel: %s, status: %.2lf \n", i, j, map[i][j].topography, map[i][j].fuel, map[i][j].status);
        }
        printf("\n");
    }
}

double burned_value(){
    //mulig anden funktion
    //switch hvis status er n er den ikke i brænd osv.
    //en funktion som tager værdien af cellens status fra arryet og oversætter til en repræsentation af celle-status (eks, n for ikke i brænd)
    //tager imod en pointer til arrayet og retunere en char (nok en swich)
    //den kaldes ind i print_kort funktionen
}

sim_loop()
{

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
// Implementation of example function

