#include <mogal/gafactorysingleobjective.h>
#include <mogal/genome.h>
#include <mogal/geneticalgorithm.h>
#include <serut/serializationinterface.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "constants.h"
#include "globals.h"
#include "mingenome.h"
#include "mingafactory.h"
#include "mingafactoryparams.h"
#include "simplerndgen.h"
#include "test.h"
#include "utils.h"

float dij[DIJ_Y][DIJ_X];
int minDose[DIM_Y][DIM_X];
int maxDose[DIM_Y][DIM_X];
int voiWeights[VOIS] = {0};
int voiData[DIM_Y][DIM_X];

int main(void)
{
	mogal::GeneticAlgorithm ga;
	MinGAFactory gaFactory;

	srand(time(0));

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

	// calculate relative VOI weights
	for (int row = 0; row < DIM_X; ++row)
	{
		for (int col = 0; col < DIM_Y-1; ++col)
		{
			//std::cout << voiData[row][col] << std::endl;
			voiWeights[voiData[col][row]] += 1;
		}
	}

	for (int weights = 0; weights < VOIS; ++ weights)
	{
		std::cout << voiWeights[weights] << std::endl;
	}

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

	// instantiate factoryparams
	MinGAFactoryParams factoryParams;

	if (!gaFactory.init(&factoryParams))
	{
		std::cerr << gaFactory.getErrorString() << std::endl;
		return -1;
	}

	if (!ga.run(gaFactory, 128))
	{
		std::cerr << ga.getErrorString() << std::endl;
		return -1;
	}

	std::list<mogal::Genome *> bestGenomes;
	ga.getBestGenomes(bestGenomes);
	Utils::printAndSaveBestGenomes(bestGenomes.front(), ga.getCurrentGeneration());
	Utils::saveDoseMatrix(bestGenomes.front(), ga.getCurrentGeneration());
	/*
	std::list<mogal::Genome *> bestGenomes;
	std::list<mogal::Genome *>::const_iterator it;

	ga.getBestGenomes(bestGenomes);

	std::cout << "Best genomes: " << std::endl;
	
	for (it = bestGenomes.begin() ; it != bestGenomes.end() ; it++)
	{
		const mogal::Genome *pGenome = *it;
		MinGenome *mGenome = (MinGenome *) *it;
		
		std::cout << "  " << pGenome->getFitnessDescription() << std::endl;
		std::cout << "  " << mGenome->getBixelweightsDescription() << std::endl;
		mGenome->writeToCSVFile("../problemset/newbw.csv", mGenome->getBixelweights());
	}
	*/
	return 0;
}

