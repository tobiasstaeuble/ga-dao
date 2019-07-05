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

int myRank;

int main(int argc, char *argv[])
{

	// MPI setup
	int worldSize;

	MPI_Init(&argc, &argv);
	MPI_Comm_size( MPI_COMM_WORLD, &worldSize);

	if (worldSize <= 1)
	{
		std::cerr << "Need more than one process to be able to work" << std::endl;
		MPI_Abort(MPI_COMM_WORLD, -1);
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);


	// does it make sense to make this static?
	Utils util;
	util.loadConfiguration();
	util.loadData();
	util.calculateRelativeWeights();
	util.defineObjectives();

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



	// algorithm and factory
	mogal::MPIGeneticAlgorithm ga;
	
	// ga Params: 
	// m_beta = 2.5 (selection presure)
	// m_elitism = true
	// m_alwaysIncludeBest = true
	// m_crossoverRate = 0.9

	mogal::GeneticAlgorithmParams gaParams(2.5, true, true, 0.9);
	MinGAFactory gaFactory;

	// time(0) is seconds since epoch, bad for MPI as we start all processes at same time
	srand(time(0) + myRank);


	// instantiate and init factoryparams
	MinGAFactoryParams factoryParams;
	gaFactory.init(&factoryParams);

	// rank 0 vs other ranks
	if (myRank == 0)
	{
		std::cout << "Rank " << myRank << " starting..." << std::endl;
		if (!ga.runMPI(gaFactory, NUM_GENOMES, &gaParams))
		{
			std::cerr << ga.getErrorString() << std::endl;
			MPI_Abort(MPI_COMM_WORLD, -1);
		}

		// finished calculation, get best genomes
		std::list<mogal::Genome *> bestGenomes;
		ga.getBestGenomes(bestGenomes);
		std::list<mogal::Genome *>::const_iterator it;

		if (OUTPUT_ON)
		{
			Utils::printAndSaveBestGenomes(bestGenomes.front(), ga.getCurrentGeneration());
			Utils::saveDoseMatrix(bestGenomes.front(), ga.getCurrentGeneration());
		}
		
		if (BATCH_ON)
		{
			Utils::saveResultBatchMode(bestGenomes);
		}

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

