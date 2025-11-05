#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "example_library.h"


int main(void) {
    // Small program that uses the example library.
    int x, y;
    scanf("%d %d", &x, &y);
    printf("%d + %d = %d\n", x, y, add(x, y));
    return 0;
}
