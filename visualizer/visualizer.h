#ifndef VISUALIZER_H
#define VISUALIZER_H

// Function to output the map in a html file
void output_and_open_html(int simulation_run_count,  map_t* map, double time_spent, int input_time, int all_time);

//subfunctions of output_and_open_html
void generate_html_file_name_and_path(char *filename, const char *OUTPUT_DIR, int simulation_run_count, int all_time);
void write_map_to_html(map_t* map, const char* filename, double time_spent);
void open_html_map_sys_dependent(char* filename, int input_time, int all_time);

#endif // VISUALIZER_H