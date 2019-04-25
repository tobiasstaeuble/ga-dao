#ifndef MINGAFACTORY_H
#define MINGAFACTORY_H 

#include <mogal/gafactorysingleobjective.h>
#include <mogal/genome.h>
#include <mogal/geneticalgorithm.h>

#include "constants.h"
#include "mingafactoryparams.h"
#include "simplerndgen.h"
#include "mingenome.h"
#include "test.h"
#include "utils.h"
#include <iostream>

class MinGAFactory : public mogal::GAFactorySingleObjective
{
public:
	MinGAFactory();
	~MinGAFactory();

	mogal::GAFactoryParams *createParamsInstance() const;

	bool init(const mogal::GAFactoryParams *pParams);
	const mogal::GAFactoryParams *getCurrentParameters() const;

	const mogal::RandomNumberGenerator *getRandomNumberGenerator() const
	{
		return &m_rndGen;
	}

	mogal::Genome *createNewGenome() const;
	size_t getMaximalGenomeSize() const;
	size_t getMaximalFitnessSize() const;
	bool writeGenome(serut::SerializationInterface &si, const mogal::Genome *pGenome) const;
	bool writeGenomeFitness(serut::SerializationInterface &si, const mogal::Genome *pGenome) const;
	bool writeCommonGenerationInfo(serut::SerializationInterface &si) const;
	bool readGenome(serut::SerializationInterface &si, mogal::Genome **pGenome) const;
	bool readGenomeFitness(serut::SerializationInterface &si, mogal::Genome *pGenome) const;
	bool readCommonGenerationInfo(serut::SerializationInterface &si);

	void onGeneticAlgorithmStep(int generation, bool *generationInfoChanged, bool *stopAlgorithm);
	void onGeneticAlgorithmStart();
	void onGeneticAlgorithmStop();
	void onSortedPopulation(const std::vector<mogal::GenomeWrapper> &population);
private:
	MinGAFactoryParams m_factoryParams;
	SimpleRndGen m_rndGen;
};

#endif