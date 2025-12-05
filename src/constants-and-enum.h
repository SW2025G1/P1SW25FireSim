//safe, maximal, setting for max map size
#define MAX_MAP_SIZE 1000

// Colors for fire status:
#define RED_BG    "\033[41m"    // Red background for burnt (Status >= 1.0)
#define GREEN_BG  "\033[42m"    // Green is for not burning (Status == 0.0)
#define YELLOW_BG "\033[43m"    // Yellow is for currently burning (0.0 < Status < 1.0)
#define RESET     "\033[0m"     // IMPORTANT: This resets the color and background back to the standard setting

//Optimization adjustables
#define TIME_STEP 0.5
#define CELL_WIDTH 5

//Symbolic constants
#define DIRECTIONS_AMOUNT 8
#define SQRT_OF_2 1.41421356

//Fuel model constants
#define TL1_MOISTURE_EXTINCTION 0.30
#define TL1_BASE_BASE_RATE 0.064
#define TL1_WIND_SCALING_RATIO 0.15 //Dense fuel packing ratio means lesser wind scaling ratio than TU1
#define TL1_SLOPE_SCALING_RATIO 0.05 //Dense fuel packing reatio means also lesser thermal penetration efficiency uphill
#define TU1_MOISTURE_EXTINCTION 0.20
#define TU1_BASE_BASE_RATE 0.140
#define TU1_WIND_SCALING_RATIO 0.35 //TU1 has a smaller packing ratio meaning the wind effect on fire spread is greater
#define TU1_SLOPE_SCALING_RATIO 0.20

//for the progress bar print function
#define BAR_WIDTH 50