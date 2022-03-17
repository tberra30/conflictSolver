#ifndef CONSTANTS_H
#define CONSTANTS_H

// Constants
#define TO_MILLISEC 1000     // millisec
#define TO_SEC 60            // sec
#define TO_PERCENTAGE 100.0  // %
#define SEP0 5               //Nm
#define D_SEP 1              // Nm/min
#define COMETE_SPACING 3     //Nm
#define WIDTH 598            //Px
#define HEIGHT 598           //Px
#define H_PRED 10            //min

// Default values
#define DEFAULT_SPEED_VALUE 20        // 1s (real) = 20s (simu)
#define UPDATE_TIME_APP 50            // Application update time (ms) (50 => update 20x each sec)
#define UPDATE_TIME_ALGO_GEN 60000    // AlgoGen update time (ms) (300000 => update 1x each 5 min)

// Default scenario config
#define TOTAL_TIME_SCENARIO 60 //min
#define MIN_SEP 6              //min
#define MAX_SEP 9              //min
#define MIN_SPD 420             //min
#define MAX_SPD 510             //min
#define WINDOW_HEIGHT 100       //Nm
#define WINDOW_WIDTH 100        //Nm

#endif // CONSTANTS_H
