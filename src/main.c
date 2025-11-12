#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "example_library.h"


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
        //

    //De parametre som indlæses fra filen
        //Topografi : elevation (double)
        //Skovtyper (en string med én type, alle samme værdier - en anden med den anden type) ID for brændseltypen
        //Status (en værdi som representere om cellen: ikke brænder,, brænder eller er udbrændt)

int main(void) {

    //initalisere et array som vi kan scanne følgende ind i: (data)
        //2D array grid (af structs?)

    //tag noget data ind for kortet/det geografiske område  - topografien (linear binary search)
        //1. åbne fil (filnavn) indholdene: tekstfil, med datafelter for hver celle
        //2. for loop
            //tager det antal data felter der skal være i sctructen og læser ind (fscanf?) på de rigtige felter i cellens struct
        //3. Gentages for alle cellerne og loopet slutter - filen lukkes

    //Funktionen print_kort(size_of_grid, struct* array)
        //1. for loop (ydre) til at printe linjerne (den bliver ved med at printe til der ikke er flere rækker (size_of_grid))
            //indre loop: for hver række skal den printe repræsentation af hver celle-status
        //2. status for hvor lang tid der er gået

    //mulig anden funktion
        //swich hvis status er n er den ikke i brænd osv.
        //en funktion som tager værdien af cellens status fra arryet og oversætter til en repræsentation af celle-status (eks, n for ikke i brænd)
        //tager imod en pointer til arrayet og retunere en char (nok en swich)
        //den kaldes ind i print_kort funktionen

    //================================================================================ kommet her til :)


        //print funktion skal tage en pointer til arrayet som input - så den kan printe ud fra det
            //Det den skal printe er en reprensentation af den funktion det er

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