#ifndef MINGAFACTORYPARAMS_H
#define MINGAFACTORYPARAMS_H

#include <mogal/gafactorysingleobjective.h>
#include <mogal/genome.h>
#include <mogal/geneticalgorithm.h>

class MinGAFactoryParams : public mogal::GAFactoryParams
{
public:
	MinGAFactoryParams();
	~MinGAFactoryParams();
	bool write(serut::SerializationInterface &si) const;
	bool read(serut::SerializationInterface &si);

	float *dij;
	int *minDose;
	int *maxDose;
};

#endif