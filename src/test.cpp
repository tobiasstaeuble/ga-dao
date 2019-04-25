#include <mogal/gafactorysingleobjective.h>
#include <mogal/genome.h>
#include <mogal/mpigeneticalgorithm.h>
#include <serut/serializationinterface.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <mpi.h>
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

int main(int argc, char *argv[])
{

	// MPI setup
	int worldSize, myRank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size( MPI_COMM_WORLD, &worldSize);

	if (worldSize <= 1)
	{
		std::cerr << "Need more than one process to be able to work" << std::endl;
		MPI_Abort(MPI_COMM_WORLD, -1);
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

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

	if (myRank == 0) {
		std::cout << "Preparing data and MPI environment..." << std::endl;
		std::cout << "MPI processes: " << worldSize << std::endl;
		std::cout << "Genomes per generation: " << NUM_GENOMES << std::endl;
		std::cout << "Generations: " << GENERATIONS << std::endl;

		for (int weights = 0; weights < VOIS; ++ weights)
		{
			std::cout << "VOI (" << std::to_string(weights) << ") weight: " << voiWeights[weights] << std::endl;
		}
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

	// algorithm and factory
	mogal::MPIGeneticAlgorithm ga;
	MinGAFactory gaFactory;

	// time(0) is seconds since epoch, bad for MPI as we start all processes at same time
	srand(time(0) + myRank);

	// time(0) is in

	// instantiate and init factoryparams
	MinGAFactoryParams factoryParams;
	gaFactory.init(&factoryParams);

	// rank 0 vs other ranks
	if (myRank == 0)
	{
		std::cout << "Rank " << myRank << " starting..." << std::endl;
		if (!ga.runMPI(gaFactory, NUM_GENOMES))
		{
			std::cerr << ga.getErrorString() << std::endl;
			MPI_Abort(MPI_COMM_WORLD, -1);
		}

		// finished calculation, get best genomes
		std::list<mogal::Genome *> bestGenomes;

		ga.getBestGenomes(bestGenomes);

		std::list<mogal::Genome *>::const_iterator it;

		ga.getBestGenomes(bestGenomes);
		Utils::printAndSaveBestGenomes(bestGenomes.front(), ga.getCurrentGeneration());
		Utils::saveDoseMatrix(bestGenomes.front(), ga.getCurrentGeneration());

	} else {
		std::cout << "Rank " << myRank << " starting..." << std::endl;
		if (!ga.runMPIHelper(gaFactory))
		{
			std::cerr << ga.getErrorString() << std::endl;
			MPI_Abort(MPI_COMM_WORLD, -1);
		}
	}

	MPI_Finalize();

	return 0;
}

