#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"
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

struct cell_t {
    double topography;
    char fuel[4];
    double status;
};

typedef struct cell_t cell_t;



int main(void) {
    char* file_path = get_file_path_from_user();
    FILE* input_file = open_data(file_path);
    //Initialisere vores array ved brug af struct. Vi sætter SIZE_OF_MAP * SIZE_OF_MAP for, at få 2D array grid
      // alle starter med status 0 på alle celler ud over den der starter med at brænde, denne starter med værdien 1
    //cell_t map[SIZE_OF_MAP][SIZE_OF_MAP];

    //tag noget data ind for kortet/det geografiske område  - topografien (linear binary search)
        //1. åbne fil (filnavn) indholdene: tekstfil, med datafelter for hver celle
            //"r" betyder at den læser filen i read-mode og derfor ikke ændre i den
        //2. for loop
            //tager det antal data felter der skal være i sctructen og læser ind (fscanf?) på de rigtige felter i cellens struct
        //3. Gentages for alle cellerne og loopet slutter - filen lukkes
//indlæs size of map fra starten af datafilen til en variabel
    for (int i = 0; i < SIZE_OF_MAP; i++) {
        for (int j = 0; j < SIZE_OF_MAP; j++) {
            fscanf(fptr, "%lf", &map[i][j].topography);
            fscanf(fptr, "%3s", map[i][j].fuel);
            fscanf(fptr, "%lf", &map[i][j].status);
        }
    }
    fclose(fptr);

    //Funktionen print_kort(size_of_grid, struct* array)
        //1. for loop (ydre) til at printe linjerne (den bliver ved med at printe til der ikke er flere rækker (size_of_grid))
            //indre loop: for hver række skal den printe repræsentation af hver celle-status
        //2. status for hvor lang tid der er gået
    for (int i = 0; i < SIZE_OF_MAP; i++) {
        for (int j = 0; j < SIZE_OF_MAP; j++) {
           if (map[i][j].status < 1) {
               printf(".  ");
           } else {
               printf("F  ");
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

    //mulig anden funktion
        //swich hvis status er n er den ikke i brænd osv.
        //en funktion som tager værdien af cellens status fra arryet og oversætter til en repræsentation af celle-status (eks, n for ikke i brænd)
        //tager imod en pointer til arrayet og retunere en char (nok en swich)
        //den kaldes ind i print_kort funktionen

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

//sidekommentar:

//kortfilden
    //ilden starter altid fra centrum af kort - spørger ikke brugeren men sættes som standard
