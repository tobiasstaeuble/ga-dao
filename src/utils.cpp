#include <mogal/genome.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include "constants.h"
#include "globals.h"
#include "mingenome.h"
#include "utils.h"
#include "angle.h"
#include <fstream>
#include <sstream>

float dij[DIJ_Y][DIJ_X];
int minDose[DIM_Y][DIM_X];
int maxDose[DIM_Y][DIM_X];
int voiWeights[VOIS] = {0};
int voiData[DIM_Y][DIM_X];

void Utils::printAndSaveBestGenomes(mogal::Genome * bestGenome, int generation) 
{
	std::cout << "Best genome: " << std::endl;
	MinGenome *mGenome = (MinGenome *) bestGenome;
	std::cout << "  " << bestGenome->getFitnessDescription() << std::endl;
	std::cout << "  " << mGenome->getBixelweightsDescription() << std::endl;
	std::cout << "  " << mGenome->getTimingDescription() << std::endl;
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

bool Utils::loadData()
{
	// load data from csv (dij, vois)

	std::ifstream file(INPUT_PATH + IN_DIJ);

	for(int row = 0; row < DIJ_X; ++row)
	{
	    std::string line;
	    std::getline(file, line);

	    if ( !file.good() ) 
	        break;

	    std::stringstream iss(line);

	    for (int col = 0; col < DIJ_Y; ++col)
	    {
	        std::string val;
	        std::getline(iss, val, ',');
	        if ( !iss.good() ) 
	            break;
	        std::stringstream convertor(val);
	        convertor >> dij[col][row];
	    }
	}
	
	file.close();
	file.clear();


	file.open(INPUT_PATH + IN_VOI);
	

	for(int row = 0; row < DIM_X; ++row)
	{
	    std::string line;
	    std::getline(file, line);
	    if ( !file.good() ) 
	        break;

	    std::stringstream iss(line);

	    for (int col = 0; col < DIM_Y; ++col)
	    {
	        std::string val;
	        std::getline(iss, val, ',');
	        if ( !iss.good() ) 
	            break;
	        std::stringstream convertor(val);
	        convertor >> voiData[col][row];
	    }
	}

	file.close();
	file.clear();
	return true;
}

bool Utils::calculateRelativeWeights()
{
	// calculate relative VOI weights
	for (int row = 0; row < DIM_X; ++row)
	{
		for (int col = 0; col < DIM_Y-1; ++col)
		{
			//std::cout << voiData[row][col] << std::endl;
			voiWeights[voiData[col][row]] += 1;
		}
	}

	return true;
}

bool Utils::defineObjectives()
{
	// define objectives
	for(int row = 0; row < DIM_X; ++row)
	{
		for (int col = 0; col < DIM_Y; ++col)
		{
			// tumor (6)
			if (voiData[col][row] == 6)
			{
				minDose[col][row] = 50;
				maxDose[col][row] = 70;
			}
			// normal tissue (1) & lungs (2,3) & spinal cord (4) & esophagus (5)
			if (voiData[col][row] >= 1 && voiData[col][row] <= 5)
			{
				maxDose[col][row] = 10;
			}

			// non-tissue surrounding area = ignore overdosing
			if (voiData[col][row] == 0) {
				maxDose[col][row] = 500;
			}


			// spinal cord
			//if (voiData[row][col] == 4)
			//{
			//	maxDose[row][col] = 8;
			//}
		}
	}
	return true;
}