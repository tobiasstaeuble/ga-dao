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
#include "INIReader.h"

float dij[DIJ_Y][DIJ_X];
int minDose[DIM_Y][DIM_X];
int maxDose[DIM_Y][DIM_X];
int voiWeights[VOIS] = {0};
int voiData[DIM_Y][DIM_X];
int GENERATIONS, NUM_GENOMES, OD_PEN, UD_PEN, BATCH_NUM;
std::string INPUT_PATH, OUTPUT_PATH, IN_DIJ, IN_VOI, OUT_DOSE, OUT_BW, OUT_BATCH;
float MUTATION_RATE, CROSSOVER_RATE, RESET_RATE, WEIGHT_DOSE, WEIGHT_TIME;
bool OUTPUT_ON, BATCH_ON, TERMINAL_ON;

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

void Utils::saveResultBatchMode(std::list<mogal::Genome *> bestGenomes)
{
	std::list<mogal::Genome *>::const_iterator it;
	ostringstream out, num;

	for (it = bestGenomes.begin() ; it != bestGenomes.end() ; it++)
		{
			std::vector<std::string> row;
			const mogal::Genome *pGenome = *it;
			MinGenome *pMinGenome = (MinGenome *)pGenome;
			pMinGenome->calculateFitness();
			out << pMinGenome->getFitness(0) << ";" << pMinGenome->getFitness(1) << "\n";
		}
	num << std::setw(5) << std::setfill('0') << std::to_string(BATCH_NUM);
	ofstream file(OUTPUT_PATH + OUT_BATCH + "_" + num.str());
    file << out.str();
}

bool Utils::loadConfiguration()
{
	INIReader reader("../config.ini");

	if (reader.ParseError() != 0) {
        std::cout << "Can't load 'test.ini'\n";
        return 1;
    }

	std::cout << "Config loaded from 'config.ini'" << "\n";

	GENERATIONS 		= reader.GetInteger("genome", "GENERATIONS", 400);
	NUM_GENOMES 		= reader.GetInteger("genome", "NUM_GENOMES", 256);
	MUTATION_RATE 		= reader.GetReal("genome", "MUTATION_RATE", 0.0);
	CROSSOVER_RATE 		= reader.GetReal("genome", "CROSSOVER_RATE", 0.0);
	RESET_RATE 			= reader.GetReal("genome", "RESET_RATE", 0.0);

	OD_PEN 				= reader.GetInteger("dosage", "OD_PEN", 50);
	UD_PEN 				= reader.GetInteger("dosage", "UD_PEN", 70);

	INPUT_PATH	        = reader.Get("io", "INPUT_PATH", "UNKNOWN");
	OUTPUT_PATH	        = reader.Get("io", "OUTPUT_PATH", "UNKNOWN");
	IN_DIJ	       		= reader.Get("io", "IN_DIJ", "UNKNOWN");
	IN_VOI	        	= reader.Get("io", "IN_VOI", "UNKNOWN");
	OUT_DOSE	        = reader.Get("io", "OUT_DOSE", "UNKNOWN");
	OUT_BW	        	= reader.Get("io", "OUT_BW", "UNKNOWN");
	OUT_BATCH        	= reader.Get("io", "OUT_BATCH", "UNKNOWN");
	OUTPUT_ON           = reader.GetBoolean("io", "OUTPUT_ON", true);
	BATCH_ON	        = reader.GetBoolean("io", "BATCH_ON", false);

	WEIGHT_DOSE         = reader.GetReal("multiobjective", "WEIGHT_DOSE", 1.0);
	WEIGHT_TIME         = reader.GetReal("multiobjective", "WEIGHT_TIME", 1.0);

    return 0;
}

bool Utils::loadData()
{
	// load data from csv (dij, vois)
	std::cout << "Input: " << INPUT_PATH  << IN_DIJ << std::endl;
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