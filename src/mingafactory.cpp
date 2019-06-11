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
		angles.push_back(Angle(i, 360/NUM_ANGLES*i));
	}
	return new MinGenome(angles, (MinGAFactory *)this);

}

size_t MinGAFactory::getMaximalGenomeSize() const
{
	return sizeof(float)*NUM_CONFIGS*NUM_ANGLES*3; // for angles matrix

	// This function is only needed if you'll be using distributed calculation.
	// In that case, it should return the maximum number of bytes a genome will
	// need for serialization.
	// return 0;
}

size_t MinGAFactory::getMaximalFitnessSize() const
{
	return sizeof(double);
	// This function is only needed if you'll be using distributed calculation.
	// In that case, it should return the maximum number of bytes the fitness 
	// measure(s) need for serialization.
	return 0;
}

bool MinGAFactory::writeGenome(serut::SerializationInterface &si, const mogal::Genome *pGenome) const
{
	// This is only needed for distributed calculation
	const MinGenome *pMinGenome = (const MinGenome *)pGenome;	

	std::vector<float> angleData = pMinGenome->serializeAngles();

	if (!si.writeFloats(angleData))
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

	if (!si.writeDouble(pMinGenome->m_fitness))
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
	std::vector<float> angleData(getMaximalGenomeSize()/sizeof(float));
	if (!si.readFloats(angleData))
	{
		setErrorString("Couldn't read genome data");
		return false;
	}
	float* ptr = &angleData[0];

	// deserialize angle data
	std::vector<Angle> nAngles;
	nAngles.resize(NUM_ANGLES);
	//std::cout << "reading: " << std::endl;
	//for (auto i: angleData)
 	//	std::cout << i << ' ';
	for (int i = 0; i < NUM_ANGLES; i++) 
	{
		for (int j = 0; j < NUM_CONFIGS; j++)
		{
			nAngles[i].configurations[j].LL = static_cast<int>(angleData[i*NUM_CONFIGS*3+j*3]);
			nAngles[i].configurations[j].RL = static_cast<int>(angleData[i*NUM_CONFIGS*3+j*3+1]);
			nAngles[i].configurations[j].time = angleData[i*NUM_CONFIGS*3+j*3+2];
		}
	}
	*pGenome = new MinGenome(nAngles, (MinGAFactory *)this);
	return true;
}

bool MinGAFactory::readGenomeFitness(serut::SerializationInterface &si, mogal::Genome *pGenome) const
{
	// This is only needed for distributed calculation
	double fitness;

	if (!si.readDouble(&fitness))
	{
		setErrorString("Couldn't read genome fitness");
		return false;
	}

	MinGenome *pMinGenome = (MinGenome *)pGenome;
	pMinGenome->m_fitness = fitness;

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

	std::cout << "Generation " << generation << ": ";
	
	for (it = bestGenomes.begin() ; it != bestGenomes.end() ; it++)
	{
		const mogal::Genome *pGenome = *it;
		
		std::cout << "  " << pGenome->getFitnessDescription() << std::endl;
	}

	if (generation >= GENERATIONS)
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