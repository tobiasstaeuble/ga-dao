#ifndef MINGENOME_H
#define MINGENOME_H 

#include <Eigen/Dense>
#include "constants.h"
#include "globals.h"

using namespace Eigen;
using namespace std;


class MinGenome : public mogal::Genome
{
public:
	MinGenome();
	MinGenome(float bw[]);
	MinGenome(VectorXf bw);
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
};

#endif