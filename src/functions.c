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
//jubi
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
FILE* get_data_from_file(FILE *fptr) {
    //tag noget data ind for kortet/det geografiske område  - topografien (linear binary search)
    //1. åbne fil (filnavn) indholdene: tekstfil, med datafelter for hver celle
    //"r" betyder at den læser filen i read-mode og derfor ikke ændre i den
    //2. for loop
    //tager det antal data felter der skal være i sctructen og læser ind (fscanf?) på de rigtige felter i cellens struct
    //3. Gentages for alle cellerne og loopet slutter - filen lukkes
    //indlæs size of map fra starten af datafilen til en variabel
    fscanf(fptr, "%*[^\n]\n");
    for (int i = 0; i < SIZE_OF_MAP; i++) {
        for (int j = 0; j < SIZE_OF_MAP; j++) {
            fscanf(fptr, "%lf", &map[i][j].topography);
            fscanf(fptr, "%3s", map[i][j].fuel);
            fscanf(fptr, "%lf", &map[i][j].status);
        }

        fclose(fptr);
    }
}
void print_grid(size_of_grid, struct* array)
{
    //Funktionen print_kort(size_of_grid, struct* array)
    // size of grid skal kalde en anden funktion, hvor grid størrelsen ligger i
    //1. for loop (ydre) til at printe linjerne (den bliver ved med at printe til der ikke er flere rækker (size_of_grid))
    //indre loop: for hver række skal den printe repræsentation af hver celle-status
    //2. status for hvor lang tid der er gået

    for (int i = 0; i < SIZE_OF_MAP; i++) {
        for (int j = 0; j < SIZE_OF_MAP; j++) {
            if (map[i][j].status < 1) {
                printf(".  ");
            } else if (map[i][j].status >3){
                printf("B  ");
            }else {
                printf("F ");
            }
        }
        printf("\n");
    }

    //Denne del af koden beholdes til de-buggin print
    for (int i = 0; i < SIZE_OF_MAP; i++) {
        for (int j = 0; j < SIZE_OF_MAP; j++) {
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

