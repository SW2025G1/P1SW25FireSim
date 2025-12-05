#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulation.h"
#include "visualizer.h"
#define CELL_SIZE_PIXELS 3

void output_and_open_html(const int simulation_run_count,  map_t* map, const double time_spent, const int input_time, const int all_time) {
    char filename[256];
    const char *OUTPUT_DIR = "output";;
    generate_html_file_name_and_path(filename, OUTPUT_DIR, simulation_run_count, all_time);
    write_map_to_html(map, filename, time_spent);
    open_html_map_sys_dependent(filename, input_time, all_time);
}

void generate_html_file_name_and_path(char *filename, const char *OUTPUT_DIR, int simulation_run_count, int all_time) {
    //Format: frame_[antal_kørsler]_[samlet_tid_minutter].html
    sprintf(filename, "%s/frame_%04d_T%dmin.html",
            OUTPUT_DIR,
            simulation_run_count,
            all_time);
}

void open_html_map_sys_dependent(char* filename, const int input_time, const int all_time) {
    char command[512];

    char *open_command = NULL;

#if defined(_WIN32) || defined(_WIN64)
    open_command = "start";
#elif defined(__APPLE__)
    open_command = "open";
#elif defined(__linux__)
    open_command = "xdg-open";+
#endif

    if (open_command) {
        sprintf(command, "%s %s", open_command, filename);
        if (system(command) != 0) {
            printf("Warning, could not open syscommand. Open %s manually from output dir.\n", filename);
        }
    }
    printf("Time run: %d minutes (Since initial ignition: %d minutes)\n",
           input_time, all_time);
}

/**
 * @brief Convert the status data into a CSS colorcode.
 * @param cell Cell data.
 * @return A static string with CSS color code.
 */
static const char* get_color_from_cell(cell_t cell) {
    if (cell.status >= 1.0) {
        return "#FF0000"; // Red (Ignited)
    } else if (strcmp(cell.fuel, "TL1") == 0 || strcmp(cell.fuel, "TU1") == 0) {
        return "#009632"; // Green (can be ignited) //TODO Dark green for TL1? So we can make mixed maps
    } else {
        return "#323296"; // Blue: not burnable (no fuel model match) //TODO Add a river &/or bare grounds in one map??
    }
}

/**
 * @brief Writing the content of the map into a HTML-document.
 * * @param map Pointer for the structure of the map.
 * @param filename The name on the file ex. "fire_frame_1.html".
 */
void write_map_to_html(map_t* map, const char* filename, double time_spent) {
    FILE* fp;
    int size = map->size_of_map;

    fp = fopen(filename, "w");
    if (!fp) {
        perror("Error: could not write the HTML file\n");
        return;
    }

    // Writing HTML Header and CSS style
    fprintf(fp, "<!DOCTYPE html>\n");
    fprintf(fp, "<html>\n");
    fprintf(fp, "<head>\n");
    fprintf(fp, "<title>Brandsimulering Ramme %s</title>\n", filename);
    fprintf(fp, "<style>\n");
    fprintf(fp, "body { margin: 0; display: flex; justify-content: center; align-items: center; min-height: 100vh; background-color: #f0f0f0; font-family: sans-serif; }\n");
    fprintf(fp, ".grid-container { \n");

    // Defines the grid layout in order to match the dimensions of the map.
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

    fprintf(fp, "<div class=\"grid-container\">\n");

    //Making iterations over each cell and writing HTML element
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cell_t cell = map->map[i * size + j];
            const char* color = get_color_from_cell(cell);
            if (i == map->size_of_map / 2 && j == map->size_of_map / 2) {
                color = "#000000";
            }
            fprintf(fp, "<div class=\"cell\" style=\"background-color: %s;\"></div>\n", color);
        }
    }


    // get fuel model from a cell
    char fuel_model [4];
    strcpy(fuel_model, map->map[1].fuel);

    //info box print
    fprintf(fp, "</div>\n");
    fprintf(fp, "<div class=\"info-box\">\n");
    fprintf(fp, "<h2>Overview</h2>\n");
    fprintf(fp, "<p>Cells size of grid: %d x %d</p>\n", size, size);
    fprintf(fp, "<p>Cell pixels: %dpx</p>\n", CELL_SIZE_PIXELS);
    fprintf(fp, "<p>Fuel model selected: %s</p>\n", fuel_model);
    fprintf(fp, "<p>Time step selected: %.2lf minutes</p>\n", TIME_STEP);
    fprintf(fp, "<p>Run time for this sim loop : %.2lf seconds</p>\n", time_spent);
    fprintf(fp, "</div>\n");

    //This ends HTML
    fprintf(fp, "</body>\n");
    fprintf(fp, "</html>\n");

    fclose(fp);
    printf("HTML file '%s' with %dx%d cells is ready to open in browser.\n", filename, size, size);
}