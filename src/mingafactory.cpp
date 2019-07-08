#include <limits>
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
	std::vector<Angle> angles;

	for(int i = 0; i < NUM_ANGLES; i++) {
		angles.push_back(Angle(i, 360/NUM_ANGLES*i, (MinGAFactory *)this));
	}
	return new MinGenome(angles, (MinGAFactory *)this);

}

size_t MinGAFactory::getMaximalGenomeSize() const
{
	return sizeof(double)*NUM_CONFIGS*NUM_ANGLES*3; // for angles matrix

	// This function is only needed if you'll be using distributed calculation.
	// In that case, it should return the maximum number of bytes a genome will
	// need for serialization.
	// return 0;
}

size_t MinGAFactory::getMaximalFitnessSize() const
{
	return sizeof(double)*2;
	// This function is only needed if you'll be using distributed calculation.
	// In that case, it should return the maximum number of bytes the fitness 
	// measure(s) need for serialization.
	return 0;
}

size_t MinGAFactory::getNumberOfFitnessComponents() const
{
	return 2;
}

bool MinGAFactory::writeGenome(serut::SerializationInterface &si, const mogal::Genome *pGenome) const
{
	// This is only needed for distributed calculation
	const MinGenome *pMinGenome = (const MinGenome *)pGenome;	

	std::vector<double> angleData = pMinGenome->serializeAngles();

	if (!si.writeDoubles(angleData))
	{
		setErrorString("Couldn't write genome data");
		return false;
	}

	return true;
}

bool MinGAFactory::writeGenomeFitness(serut::SerializationInterface &si, const mogal::Genome *pGenome) const
{
	// This is only needed for distributed calculation
	const MinGenome *pMinGenome = (const MinGenome *)pGenome;	
	std::vector<double> fitnessData (std::begin(pMinGenome->m_fitness), std::end(pMinGenome->m_fitness));
	if (!si.writeDoubles(fitnessData))
	{
		setErrorString("Couldn't write genome fitness");
		return false;
	}
	return true;
}

bool MinGAFactory::writeCommonGenerationInfo(serut::SerializationInterface &si) const
{
	// This is only needed for distributed calculation
	// setErrorString("Serialization is not implemented yet");
	return true;
}

bool MinGAFactory::readGenome(serut::SerializationInterface &si, mogal::Genome **pGenome) const
{

	// This is only needed for distributed calculation
	std::vector<double> angleData(getMaximalGenomeSize()/sizeof(double));
	if (!si.readDoubles(angleData))
	{
		setErrorString("Couldn't read genome data");
		return false;
	}
	double* ptr = &angleData[0];

	// deserialize angle data
	std::vector<Angle> nAngles;
	nAngles.resize(NUM_ANGLES, Angle((MinGAFactory *)this));
	//std::cout << "reading: " << std::endl;
	//for (auto i: angleData)
 	//	std::cout << i << ' ';
	for (int i = 0; i < NUM_ANGLES; i++) 
	{
		for (int j = 0; j < NUM_CONFIGS; j++)
		{	
			//std::cout << "i/j = " << i << "/" << j << ": LL = " << static_cast<int>(angleData[i*NUM_CONFIGS*3+j*3])
			//<< " / RL = " << static_cast<int>(angleData[i*NUM_CONFIGS*3+j*3+1]) << "/ T = " << angleData[i*NUM_CONFIGS*3+j*3+2] << std::endl;
			nAngles[i].configurations[j].LL = static_cast<int>(angleData[i*NUM_CONFIGS*3+j*3]);
			nAngles[i].configurations[j].RL = static_cast<int>(angleData[i*NUM_CONFIGS*3+j*3+1]);
			nAngles[i].configurations[j].time = angleData[i*NUM_CONFIGS*3+j*3+2];
			nAngles[i].gaFactory = (MinGAFactory *)this;
		}
	}
	*pGenome = new MinGenome(nAngles, (MinGAFactory *)this);
	return true;
}

bool MinGAFactory::readGenomeFitness(serut::SerializationInterface &si, mogal::Genome *pGenome) const
{
	// This is only needed for distributed calculation
	double fitness;
	std::vector<double> fitnessData (getMaximalFitnessSize()/sizeof(double));
	if (!si.readDoubles(fitnessData))
	{
		setErrorString("Couldn't read genome fitness");
		return false;
	}

	MinGenome *pMinGenome = (MinGenome *)pGenome;
	std::copy(fitnessData.begin(), fitnessData.end(), pMinGenome->m_fitness);

	return true;
}

bool MinGAFactory::readCommonGenerationInfo(serut::SerializationInterface &si)
{
	// This is only needed for distributed calculation
	// setErrorString("Serialization is not implemented yet");
	return true;
}

void MinGAFactory::onGeneticAlgorithmStep(int generation, bool *generationInfoChanged, bool *pStopAlgorithm)
{ 
	std::list<mogal::Genome *> bestGenomes;
	std::list<mogal::Genome *>::const_iterator it;

	getCurrentAlgorithm()->getBestGenomes(bestGenomes);

	if (TERMINAL_ON)
	{
		std::cout << "Generation " << generation << ": \n";
		for (it = bestGenomes.begin() ; it != bestGenomes.end() ; it++)
		{
			const mogal::Genome *pGenome = *it;
			// recalculate fitness again because:
			// - bixelweights is not being exchanged between processes (it is derived)
			// - no guarantee that calculateFitness is called before this onGeneticAlgorithmStep
			// this can maybe be optimized..
			MinGenome *pMinGenome = (MinGenome *)pGenome;
			pMinGenome->calculateFitness();
			std::cout << "  " << pGenome->getFitnessDescription() << std::endl;
		}
	}

	if (generation >= GENERATIONS)
		*pStopAlgorithm = true;

	if (generation % 10 == 0 && OUTPUT_ON) {
		Utils::printAndSaveBestGenomes(selectPreferredGenome(bestGenomes), generation);
		Utils::saveDoseMatrix(selectPreferredGenome(bestGenomes), generation);
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

mogal::Genome *MinGAFactory::selectPreferredGenome(const std::list<mogal::Genome *> &bestGenomes) const
{
	// Getting preferred genome
	// Because the two objectives are weighted, use the combined fitness value
	double bestFitness = std::numeric_limits<double>::infinity();
	mogal::Genome *bestGenome = bestGenomes.front();

	std::list<mogal::Genome *>::const_iterator it;

	for (it = bestGenomes.begin() ; it != bestGenomes.end() ; it++)
	{
		const mogal::Genome *pGenome = *it;
		MinGenome *pMinGenome = (MinGenome *)pGenome;
		//pMinGenome->calculateFitness();
		double f = pMinGenome->getFitnessF(0) + pMinGenome->getFitnessF(1);
		if (f < bestFitness)
		{
			bestFitness = f;
			bestGenome = *it;
		}
	}
	std::cout << "selecting genome with f-sum " << bestFitness << std::endl;
	return bestGenome;
}