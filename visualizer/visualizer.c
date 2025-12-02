#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulation.h"
#include "visualizer.h"

// Definerer størrelsen af hver celle i pixels (f.eks. 4x4)
#define CELL_SIZE_PIXELS 4

/**
 * @brief Omdanner statusdata til en CSS farvekode.
 * @param cell Cellens data.
 * @return En statisk streng med CSS farvekoden.
 */
static const char* get_color_from_cell(cell_t cell) {
    // Bemærk: Du kan forfine disse farver for bedre visualisering
    if (cell.status >= 1.0) {
        return "#646464"; // Grå (Udbrændt/Aske)
    } else if (cell.status > 0.5) {
        return "#FF6600"; // Orange (Høj ild)
    } else if (cell.status > 0.0) {
        return "#FFCC00"; // Gul (Ulmer)
    } else if (strcmp(cell.fuel, "TL1") == 0 || strcmp(cell.fuel, "TU1") == 0) {
        return "#009632"; // Grøn (Brændbar)
    } else {
        return "#323296"; // Blå (Ikke-brændbar, f.eks. Vand)
    }
}


/**
 * @brief Skriver indholdet af kortet til et HTML-dokument.
 * * @param map Pointer til kortstrukturen.
 * @param filename Navnet på filen, f.eks. "fire_frame_1.html".
 */
void write_map_to_html(map_t* map, const char* filename) {
    FILE* fp;
    int size = map->size_of_map;

    fp = fopen(filename, "w");
    if (!fp) {
        perror("Fejl: Kunne ikke oprette HTML-filen");
        return;
    }

    // --- 1. Skriv HTML Header og CSS Stil ---
    fprintf(fp, "<!DOCTYPE html>\n");
    fprintf(fp, "<html>\n");
    fprintf(fp, "<head>\n");
    fprintf(fp, "<title>Brandsimulering Ramme %s</title>\n", filename);
    fprintf(fp, "<style>\n");
    fprintf(fp, "body { margin: 0; display: flex; justify-content: center; align-items: center; min-height: 100vh; background-color: #f0f0f0; font-family: sans-serif; }\n");
    fprintf(fp, ".grid-container { \n");
    // Definer grid layoutet til at matche kortets dimensioner
    fprintf(fp, "  display: grid;\n");
    fprintf(fp, "  grid-template-columns: repeat(%d, %dpx);\n", size, CELL_SIZE_PIXELS);
    fprintf(fp, "  grid-template-rows: repeat(%d, %dpx);\n", size, CELL_SIZE_PIXELS);
    fprintf(fp, "  border: 1px solid #ccc;\n");
    fprintf(fp, "}\n");
    fprintf(fp, ".cell { width: %dpx; height: %dpx; }\n", CELL_SIZE_PIXELS, CELL_SIZE_PIXELS);
    fprintf(fp, ".info-box { position: fixed; top: 10px; right: 10px; background: white; padding: 10px; border-radius: 5px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }\n");
    fprintf(fp, "</style>\n");
    fprintf(fp, "</head>\n");
    fprintf(fp, "<body>\n");

    // --- 2. Skriv Grid Container ---
    fprintf(fp, "<div class=\"grid-container\">\n");

    // --- 3. Iterer over hver celle og skriv HTML element ---
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cell_t cell = map->map[i * size + j];
            const char* color = get_color_from_cell(cell);

            // Generer et <div> element for hver celle med den korrekte baggrundsfarve
            fprintf(fp, "<div class=\"cell\" style=\"background-color: %s;\"></div>\n", color);
        }
    }

    fprintf(fp, "</div>\n");

    // Valgfri: Tilføj lidt info om rammen
    fprintf(fp, "<div class=\"info-box\">\n");
    fprintf(fp, "<h2>Simuleringsoverblik</h2>\n");
    fprintf(fp, "<p>Størrelse: %d x %d</p>\n", size, size);
    fprintf(fp, "<p>Celle størrelse: %dpx</p>\n", CELL_SIZE_PIXELS);
    fprintf(fp, "</div>\n");

    // --- 4. Afslut HTML ---
    fprintf(fp, "</body>\n");
    fprintf(fp, "</html>\n");

    fclose(fp);
    printf("HTML filen '%s' med %dx%d celler er skrevet og klar til at åbne i browseren.\n", filename, size, size);
}