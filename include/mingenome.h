#ifndef MINGENOME_H
#define MINGENOME_H 

#include <Eigen/Dense>
#include "mingafactory.h"
#include "constants.h"
#include "globals.h"
#include "angle.h"
#include <vector>

using namespace Eigen;
using namespace std;

class MinGAFactory;


class MinGenome : public mogal::Genome
{
public:
	MinGenome(MinGAFactory *pFactory);
	MinGenome(std::vector<Angle> angles, MinGAFactory *pFactory);
	~MinGenome();

	bool calculateFitness();
	bool isFitterThan(const mogal::Genome *pGenome) const;
	void setActiveFitnessComponent(int i);
	Genome *reproduce(const mogal::Genome *pGenome) const;
	mogal::Genome *clone() const;
	void mutate();
	void writeToCSVFile(string name, MatrixXf matrix);
	void writeVectorToCSVFile(string name, std::vector<float> vec) const;

	std::string getFitnessDescription() const;
	std::string getBixelweightsDescription() const;
	std::string getTimingDescription() const;
	VectorXf getBixelweights() const;
	std::vector<float> serializeAngles() const;

private:
	int beamlets;
	VectorXf bixelweights;
	double m_fitness[2];
	int m_activeFitness;
	MinGAFactory *m_pFactory;
	std::vector<Angle> angles;
	bool genomeChanged = false;
	friend class MinGAFactory;

};

#endif