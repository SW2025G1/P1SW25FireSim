//safe, maximal, setting for max map size
#define MAX_MAP_SIZE 1000

// Colors for fire status:
#define RED_BG    "\033[41m"    // Red background for burnt (Status >= 1.0)
#define GREEN_BG  "\033[42m"    // Green is for not burning (Status == 0.0)
#define YELLOW_BG "\033[43m"    // Yellow is for currently burning (0.0 < Status < 1.0)
#define RESET     "\033[0m"     // IMPORTANT: This resets the color and background back to the standard setting

//Optimization adjustables
#define TIME_STEP 0.5
#define CELL_WIDTH 10

//Symbolic constants
#define DIRECTIONS_AMOUNT 8
#define SQRT_OF_2 1.41421356

//---------Fuel model constants----------
//TL1 (Timber Litter)
#define TL1_MOISTURE_EXTINCTION 0.30
#define TL1_BASE_BASE_RATE 0.064
#define TL1_WIND_SCALING_RATIO 0.15 //Dense fuel packing ratio means lesser wind scaling ratio than TU1
#define TL1_SLOPE_SCALING_RATIO 3 //Dense fuel packing reatio means also lesser thermal penetration efficiency uphill
#define TL1_MAX_WIND_SPEED_CALC 2.46 //the increase of wind speed effect on rate of spread caps at 2.46 m/s for TL1
//TU1 (Timber Understory)
#define TU1_MOISTURE_EXTINCTION 0.20
#define TU1_BASE_BASE_RATE 0.140
#define TU1_WIND_SCALING_RATIO 0.30 //TU1 has a smaller packing ratio meaning the wind effect on fire spread is greater
#define TU1_SLOPE_SCALING_RATIO 10
#define TU1_MAX_WIND_SPEED_CALC 8.49 //the increase of wind speed effect on rate of spread caps at 8.49 m/s for TU1
//GR1 (Short, sparse grass)
#define GR1_MOISTURE_EXTINCTION 0.15
#define GR1_BASE_BASE_RATE 0.385
#define GR1_WIND_SCALING_RATIO 1.6 //GR1 is the very high surface area fuel that is extremely sensitive to wind speed
#define GR1_SLOPE_SCALING_RATIO 25
#define GR1_MAX_WIND_SPEED_CALC 2.24 //the increase of wind speed effect on rate of spread caps at 2.24 m/s for GR1

//for the progress bar print function
#define BAR_WIDTH 50