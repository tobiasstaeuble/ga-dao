#include <mogal/genome.h>
#include <iostream>
#include <iomanip>
#include "constants.h"
#include "globals.h"
#include "mingenome.h"
#include "utils.h"


void Utils::printAndSaveBestGenomes(mogal::Genome * bestGenome, int generation) 
{
	std::cout << "Best genome: " << std::endl;
	MinGenome *mGenome = (MinGenome *) bestGenome;
	std::cout << "  " << bestGenome->getFitnessDescription() << std::endl;
	std::cout << "  " << mGenome->getBixelweightsDescription() << std::endl;
	ostringstream out;
	out << std::setw(5) << std::setfill('0') << std::to_string(generation);
	mGenome->writeToCSVFile(OUTPUT_PATH + OUT_BW + "_" + out.str(), mGenome->getBixelweights());
}

void Utils::saveDoseMatrix(mogal::Genome* bestGenome, int generation) 
{
	MinGenome *mGenome = (MinGenome *) bestGenome;
	Map<MatrixXf> dijMatrix(*dij, DIJ_X, DIJ_Y);
	MatrixXf dose(DIM_X, DIM_Y);
	dose = dijMatrix.transpose() * mGenome->getBixelweights();
	Map<ArrayXf> dose2Array(dose.data(), dose.rows()*dose.cols());
	ostringstream out;
	out << std::setw(5) << std::setfill('0') << std::to_string(generation);
	mGenome->writeToCSVFile(OUTPUT_PATH + OUT_DOSE + "_" + out.str(), dose2Array);
}