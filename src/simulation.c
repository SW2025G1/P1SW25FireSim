#include "simulation.h"
#include <math.h>
#include "functions.h"

#define TIME_STEP 5
#define DIRECTIONS_AMOUNT 8
#define CELL_WIDTH 5

void sim_loop(array_t* array, Weather* w) {
    char input_char = 'y';
    int input_time = 0;
    do {
        input_time_or_exit(&input_char, &input_time);

        for (int k = TIME_STEP; k < input_time; k += TIME_STEP) { //TODO: skal justere problemet med yderste grid-celler
            for (int i = 1; i < array->size_of_array - 1; i++) { //i sættes til 1, fordi den springer over første kolonne
                for (int j = 1; j < array->size_of_array - 1; j++) {
                    calculate_new_status(array, w, i, j, k);
                }
            }
        }
        print_grid(array);
    } while (input_char != 'x');

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

void calculate_new_status(array_t* array, Weather* w, int i, int j, int k) {
    //Fokuserer på én celle
    //8 ting
    //loop - kalder funktion der beregner fra specifik retning
    //loop starter fra sydcelle
    //sydrate
    for (int l = 0; l < DIRECTIONS_AMOUNT; l += M_PI/4) { //vinkel vi beregner på er 0 til start - lægger pi fjerdedel til pr gang
        array->map[i * array->size_of_array + j].status += rate_of_spread(array, w, i, j, k); //nabocellernes bidrag til at tillægge statusværdi til cellen
    }
}

double rate_of_spread(array_t* array, Weather* w, int i, int j, int k) {

    double base_rate_of_spread = calculate_base_rate(array->map[i * array->size_of_array + j].fuel, w->moisture_of_fuel);
    double wind_factor = calculate_wind_factor(k, w->wind_speed, w->wind_direction); //denne wind_direction skal laves om til double - se kommentar i simulation.h
    double slope_factor = calculate_slope_factor(k, i, j); //elevation fra sig selv og fra k-retning
    double total_spread_rate = calculate_total_spread_rate(base_rate_of_spread, wind_factor, slope_factor); //funktion tager foregående calculations og samler
    double ignition_time = CELL_WIDTH / total_spread_rate; //distance pr. rate - fx meter pr. min.

    return TIME_STEP / ignition_time;
}

double calculate_base_rate(char* fuel_model, double moisture_of_fuel) {
    //tage base rate - funktionen skal bruge fuel model id og moisture fra weather

    //return ;
}

double calculate_wind_direction(int wind_direction) {
    //omdan userinput: wind_direction til vinkel i radianer
        switch (wind_direction) {
            case 1: return M_PI/2;        // nord
            case 2: return M_PI/4;        // nordøst
            case 3: return 0;             // øst
            case 4: return -M_PI/4;       // sydøst
            case 5: return -M_PI/2;       // syd
            case 6: return -3*M_PI/4;     // sydvest
            case 7: return M_PI;          // vest
            case 8: return 3*M_PI/4;      // nordvest
            default: return 0;            // fallback
        }
}

double calculate_wind_factor(int k, double wind_speed, int wind_direction) {
    double wind_factor = 0;
    double C_wind = 0.20;
    double theta_i = 0; //skal ændres

    double new_wind_direction = calculate_wind_direction(wind_direction);

    wind_factor = C_wind * wind_speed * max(0, cos(new_wind_direction - theta_i));
    //hvor meget bidrager vinden til at den spreder sig hurtigerre i den angivne retning
    //k = retning - vi vil gerne beregne for denne
    //den faktiske vindretning
    //hvis den er negativ, dvs. vind ikke blæser i den retning = 0
    //hastighed som er komponenten af vinden - matematisk funktion - max(0,f(kom)) <- tager maksimale værdi mellem 2. Hvis den højre er positiv, så vælger den den. Hvis den er negativ, så vælger den 0

    //R = R_0 * (1 + theta_wind_1 i + slope factor_1 y)
    //return max(0,f(kom)); //f(kom) er beregningsresultatet - theta wind (wind factor)
}

double calculate_slope_factor(int k, int i, int j) {
    //start i = -1, j = -1
    double elevation_of_k_cell = get_neighbour_elevation(k, i, j);
    // elevation_of_k_cell = k nabocelle

    //return ;
}

double get_neighbour_elevation(int k, int i, int j) {
    //switch - 8 cases
    //finder elevationsværdi, som den skal beregne med

    //return ;
}

//Her får vi de forskellige komponenter ind i Rothermel
double calculate_total_spread_rate(double base_rate_of_spread, double wind_factor, double slope_factor) {
    return base_rate_of_spread * (1 + wind_factor + slope_factor);
}