#include "simulation.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "functions.h"
#include "../visualizer/visualizer.h"

// Definerer TIME_STEP, DIRECTIONS_AMOUNT, CELL_WIDTH, SQRT_OF_2 fra simulation.h,
// hvis de ikke allerede er defineret.
#ifndef TIME_STEP
#define TIME_STEP 5
#endif
#ifndef DIRECTIONS_AMOUNT
#define DIRECTIONS_AMOUNT 8
#endif
#ifndef CELL_WIDTH
#define CELL_WIDTH 5.0
#endif
#ifndef SQRT_OF_2
#define SQRT_OF_2 1.41421356
#endif

// Ny konstant for output-mappen
const char *OUTPUT_DIR = "output";

// -----------------------------------------------------------------------------
// IMPLEMENTERING AF CELLULÆR AUTOMATA LOGIK (CALCULATE_NEW_STATUS)
// -----------------------------------------------------------------------------
// Alle funktioner deklareret i simulation.h skal defineres herunder.

/**
 * @brief Beregner den nye status for en given celle (i, j) og nabo-antændelser.
 * * Læser fra map->map (nuværende tilstand) og skriver til map->temp_map (næste tilstand).
 * @param map Pointer til kortstrukturen (indeholder både map og temp_map)
 * @param w Pointer til vejrdata
 * @param i Rækkeindeks
 * @param j Søjleindeks
 */
void calculate_new_status(map_t* map, Weather_t* w, int i, int j) {

    // Brug indekset for at få adgang til den nuværende celle i det flade array
    int index = i * map->size_of_map + j;
    cell_t current_cell = map->map[index]; // Læs fra det gamle kort

    // Antager at cellen er brændbar hvis den har en fuel model
    if (current_cell.fuel[0] == '\0') {
        // Skriv den uændrede tilstand tilbage til temp_map for at sikre konsistens
        map->temp_map[index] = current_cell;
        return;
    }

    // ---------------------------------------------------------
    // 1. OPSTART AF NUVERENDE CELLE (DAMPENING/SLUKNING)
    // ---------------------------------------------------------

    // Hvis cellen er i brand, reducer dens status (simulerer afbrænding/slukning)
    if (current_cell.status > 0.0) {

        // Reducer status med TIME_STEP
        double new_status = current_cell.status - TIME_STEP;

        // Sørg for, at status ikke går under 0
        if (new_status <= 0.0) {
            new_status = 0.0;
            // Markér cellen som udbrændt
            // Vi kunne sætte fuel til en "udbrændt" type her, men holder os til kun status for nu.
        }

        // Opdater kun temp_map!
        map->temp_map[index].status = new_status;

    } else {
        // Hvis cellen ikke brænder (status == 0), bevar status i temp_map
        // (den kan stadig antændes af naboer senere i løkken)
        map->temp_map[index].status = 0.0;
    }

    // ---------------------------------------------------------
    // 2. TJEK FOR ANTÆNDELSE (IGNITION)
    // ---------------------------------------------------------

    // Vi tjekker kun spredning fra celler, der er FULDT antændt (status >= 1.0)
    if (current_cell.status >= 1.0) {

        // Loop gennem de 8 Moore naboer
        for (int direction_int = 0; direction_int < DIRECTIONS_AMOUNT; direction_int++) {

            direction_t direction;
            direction.direction_from_neighbor_int = direction_int;

            // Brug din direction_to_radians funktion (Stub implementeret herunder)
            direction.direction_from_neighbor_radians = direction_to_radians(direction_int);

            // Hent naboens indeks
            int neighbor_index = get_neighbor_index(map, i, j, direction_int);

            // Tjek at indekset er gyldigt (ikke en kantcelle/uden for kortet)
            if (neighbor_index != -1) {

                // Læs nabo-cellen (fra det gamle kort!)
                cell_t neighbor_cell = map->map[neighbor_index];

                // Tjek om nabo-cellen er brændbar (status == 0)
                if (neighbor_cell.status == 0.0) {

                    // --- BEREGN SPREDNINGSHASTIGHED ---

                    // 1. Calculate Total Spread Rate (R_i)
                    // Her bruger vi din status_calculator funktion, som antages at samle alle faktorer.
                    double total_spread_rate = status_calculator(map, w, i, j, direction);

                    // --- BEREGN IGNITION TIME (T_i) ---
                    double ignition_time;

                    // Find afstanden (hypotenuse for diagonaler)
                    double distance;
                    // Lige retninger (Øst, Nord, Vest, Syd) er lige (lige index 0, 2, 4, 6)
                    if (direction_int % 2 == 0) {
                        distance = CELL_WIDTH;
                    } else {
                        // Diagonale retninger (ulige index 1, 3, 5, 7)
                        distance = CELL_WIDTH * SQRT_OF_2;
                    }

                    if (total_spread_rate > 0.0) {
                        ignition_time = distance / total_spread_rate; // T = D / R
                    } else {
                        ignition_time = INFINITY;
                    }

                    // --- OPDATER ACCUMULATED TIME ---

                    // Vigtigt: Vi skal opdatere ACCUMULATED TIME for naboen i temp_map
                    // Hvis denne nye vej giver en hurtigere antændelsestid:
                    if (ignition_time < map->temp_map[neighbor_index].status) {
                         // Opdater temp_map med den nye, kortere antændelsestid
                         map->temp_map[neighbor_index].status = ignition_time;
                    }

                    // Tjek om akkumuleret tid nu er nået eller overskredet
                    // Vi akkumulerer faktisk kun den korteste tid her. Selve tidsforbruget
                    // (nedtællingen) sker bedst i sim_loop, men for nu tjekker vi blot om den
                    // korteste fundne tid er <= TIME_STEP for at antænde.

                    if (map->temp_map[neighbor_index].status <= TIME_STEP) {
                         // Branden antændes nu! Sæt status til 1.0 (fuldt i brand) i temp_map
                         map->temp_map[neighbor_index].status = 1.0;
                         map->temp_map[neighbor_index].accumulated_time_to_ignite = 0.0; // Nulstil
                    }
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// FUNKTION STUBS / IMPLEMENTATIONER
// -----------------------------------------------------------------------------

/**
 * @brief STUB: Beregner den samlede spredningshastighed (R_i = R0 * (1 + phi_w + phi_s)).
 * @details Kalder base_rate, wind_factor og slope_factor.
 */
double status_calculator(map_t* map, Weather_t* w, int i, int j, direction_t direction_from_neighbor) {
    double base_rate_of_spread = calculate_base_rate(map, w, i, j, direction_from_neighbor);
    double wind_factor = calculate_wind_factor(map, i, j, w, direction_from_neighbor);
    double slope_factor = calculate_slope_factor(map, i, j, direction_from_neighbor);

    // Bruger din definerede funktion til at beregne total spredningsrate.
    return calculate_total_spread_rate(base_rate_of_spread, wind_factor, slope_factor);
}

/**
 * @brief STUB: Beregner Base Rate of Spread (R0).
 * @details Sk