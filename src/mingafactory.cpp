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
	float bw[BEAMLETS*NUM_ANGLES];
	for (int i = 0; i < BEAMLETS*NUM_ANGLES; i++) {
		bw[i] = 1;
	}
	return new MinGenome(bw, (MinGAFactory *)this);

}

size_t MinGAFactory::getMaximalGenomeSize() const
{
	return sizeof(float)*BEAMLETS*NUM_ANGLES; // for bixelweights vector

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

	VectorXf bw = pMinGenome->getBixelweights();

	std::vector<float> vec(bw.data(), bw.data() + bw.rows() * bw.cols());

	if (!si.writeFloats(vec))
	{
		setErrorString("Couldn't write genome bixelweights");
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
	std::vector<float> bw(getMaximalGenomeSize()/sizeof(float));
	if (!si.readFloats(bw))
	{
		setErrorString("Couldn't read genome bixelweights");
		return false;
	}
	float* ptr = &bw[0];
	Eigen::Map<Eigen::VectorXf> vec(ptr, getMaximalGenomeSize()/sizeof(float));
	*pGenome = new MinGenome(vec, (MinGAFactory *)this);
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