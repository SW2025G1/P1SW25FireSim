#ifndef VISUALIZER_H
#define VISUALIZER_H

// Inkluder nødvendige structs, f.eks. map_t
#include "simulation.h" // Antager din simulation.h definerer map_t

// Funktionen til at skrive HTML-filen
void write_map_to_html(map_t* map, const char* filename);

#endif // VISUALIZER_H