#ifndef GLOBALS_H
#define GLOBALS_H 

#include "constants.h"
#include "angle.h"
#include <vector>

extern float dij[DIJ_Y][DIJ_X];
extern int minDose[DIM_Y][DIM_X];
extern int maxDose[DIM_Y][DIM_X];

extern int voiWeights[VOIS];
extern int voiData[DIM_Y][DIM_X];

extern int myRank;

// from config file
extern int GENERATIONS;
extern int NUM_GENOMES;
extern float MUTATION_RATE;
extern float CROSSOVER_RATE;
extern float RESET_RATE;

extern std::string INPUT_PATH;
extern std::string OUTPUT_PATH;
extern std::string IN_DIJ;
extern std::string IN_VOI;
extern std::string OUT_DOSE;
extern std::string OUT_BW;
extern std::string OUT_BATCH;
extern bool OUTPUT_ON;
extern bool TERMINAL_ON;
extern bool BATCH_ON;
extern int BATCH_NUM;


extern int OD_PEN;
extern int UD_PEN;

extern float WEIGHT_DOSE;
extern float WEIGHT_TIME;

#endif