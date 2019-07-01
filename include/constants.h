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

const static IOFormat CSVFormat(StreamPrecision, DontAlignCols, ", ", "\n");


#endif