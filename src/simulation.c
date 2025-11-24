#include "simulation.h"
#include "functions.h"
#define TIME_STEP 5

void sim_loop(array_t* array, Weather* w) {
    char input_char = 'y';
    int input_time = 0;
    do {
        input_time_or_exit(&input_char, &input_time);

        for (int i = TIME_STEP; i < input_time; i += TIME_STEP) {
            for (int j = 0; j < array->size_of_array; j++) {
                for (int k = 0; k < array->size_of_array; k++) {
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