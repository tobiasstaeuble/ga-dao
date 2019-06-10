#ifndef MINGENOME_H
#define MINGENOME_H 

#include <Eigen/Dense>
#include "mingafactory.h"
#include "constants.h"
#include "globals.h"
#include "angle.h"

using namespace Eigen;
using namespace std;

class MinGAFactory;


class MinGenome : public mogal::Genome
{
public:
	MinGenome(MinGAFactory *pFactory);
	MinGenome(float bw[], MinGAFactory *pFactory);
	MinGenome(VectorXf bw, MinGAFactory *pFactory);
	~MinGenome();

	bool calculateFitness();
	bool isFitterThan(const mogal::Genome *pGenome) const;
	Genome *reproduce(const mogal::Genome *pGenome) const;
	mogal::Genome *clone() const;
	void mutate();
	void writeToCSVFile(string name, MatrixXf matrix);
	
	std::string getFitnessDescription() const;
	std::string getBixelweightsDescription() const;
	VectorXf getBixelweights() const;

private:
	int beamlets;
	VectorXf bixelweights;
	double m_fitness;
	MinGAFactory *m_pFactory;
	Angle angles[NUM_ANGLES];

	friend class MinGAFactory;

};

#endif