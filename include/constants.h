#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Eigen/Dense>
using namespace Eigen;

const int DIM_X					= 82;
const int DIM_Y					= 147;
const int NUM_ANGLES			= 9;
const int BEAMLETS				= 15;
const int NUM_CONFIGS			= 10;
const int VOIS					= 7;

const int DIJ_X 				= NUM_ANGLES * BEAMLETS;
const int DIJ_Y 				= DIM_X * DIM_Y;

const int OD_PEN				= 50;
const int UD_PEN				= 70;

const int GENERATIONS			= 400;
const int NUM_GENOMES			= 256;
const float MUTATION_RATE		= 0.05;
const float CROSSOVER_RATE		= 0.05;
const float RESET_RATE			= 0.001;

// forward-slash syntax should work on windows as well as on unix-systems
const std::string INPUT_PATH	= "../problemset/";
const std::string OUTPUT_PATH	= "../output/";

const std::string IN_DIJ		= "dij.csv";
const std::string IN_VOI		= "voi.csv";

const std::string OUT_DOSE		= "dose";
const std::string OUT_BW		= "bixelweights";

const static IOFormat CSVFormat(StreamPrecision, DontAlignCols, ", ", "\n");


#endif