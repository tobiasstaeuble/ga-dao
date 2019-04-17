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
#include "constants.h"
#include "globals.h"
#include "mingenome.h"
#include "mingafactory.h"
#include "mingafactoryparams.h"
#include "simplerndgen.h"
#include <iomanip>

float dij[DIJ_Y][DIJ_X];
int minDose[DIM_Y][DIM_X];
int maxDose[DIM_Y][DIM_X];
int voiWeights[VOIS] = {0};
int voiData[DIM_Y][DIM_X];

// Factory parameters implementation

MinGAFactoryParams::MinGAFactoryParams()
{
}

MinGAFactoryParams::~MinGAFactoryParams()
{
}

bool MinGAFactoryParams::write(serut::SerializationInterface &si) const
{
	setErrorString("Serialization is not implemented yet");
	return false;
}

bool MinGAFactoryParams::read(serut::SerializationInterface &si)
{
	setErrorString("Serialization is not implemented yet");
	return false;
}

// Factory implementation

MinGAFactory::MinGAFactory()
{
}

MinGAFactory::~MinGAFactory()
{
}

mogal::GAFactoryParams *MinGAFactory::createParamsInstance() const
{
	return new MinGAFactoryParams();
}

bool MinGAFactory::init(const mogal::GAFactoryParams *pParams)
{
	// Currently no initialization is needed
	return true;
}

const mogal::GAFactoryParams *MinGAFactory::getCurrentParameters() const
{
	return &m_factoryParams;
}


mogal::Genome *MinGAFactory::createNewGenome() const
{
	float bw[BEAMLETS*ANGLES];
	for (int i = 0; i < BEAMLETS*ANGLES; i++) {
		bw[i] = 1;
	}
	return new MinGenome(bw);

}

size_t MinGAFactory::getMaximalGenomeSize() const
{
	// This function is only needed if you'll be using distributed calculation.
	// In that case, it should return the maximum number of bytes a genome will
	// need for serialization.
	return 0;
}

size_t MinGAFactory::getMaximalFitnessSize() const
{
	// This function is only needed if you'll be using distributed calculation.
	// In that case, it should return the maximum number of bytes the fitness 
	// measure(s) need for serialization.
	return 0;
}

bool MinGAFactory::writeGenome(serut::SerializationInterface &si, const mogal::Genome *pGenome) const
{
	// This is only needed for distributed calculation
	setErrorString("Serialization is not implemented yet");
	return false;
}

bool MinGAFactory::writeGenomeFitness(serut::SerializationInterface &si, const mogal::Genome *pGenome) const
{
	// This is only needed for distributed calculation
	setErrorString("Serialization is not implemented yet");
	return false;
}

bool MinGAFactory::writeCommonGenerationInfo(serut::SerializationInterface &si) const
{
	// This is only needed for distributed calculation
	setErrorString("Serialization is not implemented yet");
	return false;
}

bool MinGAFactory::readGenome(serut::SerializationInterface &si, mogal::Genome **pGenome) const
{
	// This is only needed for distributed calculation
	setErrorString("Serialization is not implemented yet");
	return false;
}

bool MinGAFactory::readGenomeFitness(serut::SerializationInterface &si, mogal::Genome *pGenome) const
{
	// This is only needed for distributed calculation
	setErrorString("Serialization is not implemented yet");
	return false;
}

bool MinGAFactory::readCommonGenerationInfo(serut::SerializationInterface &si)
{
	// This is only needed for distributed calculation
	setErrorString("Serialization is not implemented yet");
	return false;
}


void printAndSaveBestGenomes(mogal::Genome * bestGenome, int generation) 
{
	std::cout << "Best genome: " << std::endl;
	MinGenome *mGenome = (MinGenome *) bestGenome;
	std::cout << "  " << bestGenome->getFitnessDescription() << std::endl;
	std::cout << "  " << mGenome->getBixelweightsDescription() << std::endl;
	ostringstream out;
	out << std::setw(5) << std::setfill('0') << std::to_string(generation);
	mGenome->writeToCSVFile(OUTPUT_PATH + OUT_BW + "_" + out.str(), mGenome->getBixelweights());
}

void saveDoseMatrix(mogal::Genome* bestGenome, int generation) 
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


void MinGAFactory::onGeneticAlgorithmStep(int generation, bool *generationInfoChanged, bool *pStopAlgorithm)
{ 
	std::list<mogal::Genome *> bestGenomes;
	std::list<mogal::Genome *>::const_iterator it;

	getCurrentAlgorithm()->getBestGenomes(bestGenomes);

	std::cout << "Generation " << generation << ": ";
	
	for (it = bestGenomes.begin() ; it != bestGenomes.end() ; it++)
	{
		const mogal::Genome *pGenome = *it;
		
		std::cout << "  " << pGenome->getFitnessDescription() << std::endl;
	}

	if (generation >= 2000)
		*pStopAlgorithm = true;

	if (generation % 10 == 0) {
		printAndSaveBestGenomes(bestGenomes.front(), generation);
		saveDoseMatrix(bestGenomes.front(), generation);
	}
}

void MinGAFactory::onGeneticAlgorithmStart()
{
	// This gets called when the genetic algorithm is started. No 
	// implementation is necessary for the algorithm to work.
}

void MinGAFactory::onGeneticAlgorithmStop()
{
	// This gets called when the genetic algorithm is stopped. No 
	// implementation is necessary for the algorithm to work.
}

void MinGAFactory::onSortedPopulation(const std::vector<mogal::GenomeWrapper> &population)
{
	// This gets called each time the population is sorted. No
	// implementation is necessary for the algorithm to work.
}

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
	printAndSaveBestGenomes(bestGenomes.front(), ga.getCurrentGeneration());
	saveDoseMatrix(bestGenomes.front(), ga.getCurrentGeneration());
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

