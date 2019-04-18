#include "mingafactory.h"

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
		Utils::printAndSaveBestGenomes(bestGenomes.front(), generation);
		Utils::saveDoseMatrix(bestGenomes.front(), generation);
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