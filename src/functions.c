#include <math.h>
#include "functions.h"


FILE* open_data(char* file_name) {
    FILE *fptr;
    fptr = fopen("src/Data.txt", "r");
    //tilføjer tjek for, at se om filen åbnes korrekt
    if (fptr == NULL) {
        printf("Filen blev ikke åbnet korrekt\n");
        exit(EXIT_FAILURE);
    }
    return fptr;
}
//TODO: Implement user data input of relative file path (src/"..."):
char* get_file_path_from_user() {

}

// Implementation of example function

