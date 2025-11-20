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



int main(void)
{
    //Vi kan eventuelt samle det hele i functions og lave en funktion, der kalder de andre funktioner, så vi kun har en funktion i main?
    char* file_path = get_file_path_from_user();
    FILE* input_file = open_data(file_path);
    //sidekommentar:
    //kortfilden
    //ilden starter altid fra centrum af kort - spørger ikke brugeren men sættes som standard
}