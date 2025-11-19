#include <math.h>
#include "example_library.h"
FILE *fptr;
fptr = fopen("src/Data.txt", "r");

//tilføjer tjek for, at se om filen åbnes korrekt
if (fptr == NULL) {
    printf("Filen blev ikke åbnet korrekt\n");
    return 1;
}

// Implementation of example function

