#ifndef MINGAFACTORYPARAMS_H
#define MINGAFACTORYPARAMS_H

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