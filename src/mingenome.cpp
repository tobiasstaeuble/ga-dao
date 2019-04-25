#include <mogal/gafactorysingleobjective.h>
#include <mogal/genome.h>
#include <mogal/geneticalgorithm.h>
#include <serut/serializationinterface.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include "mingenome.h"
#include "globals.h"
#include <Eigen/Dense>
#include <fstream>

using namespace Eigen;
using namespace std;

// Genome implementation

MinGenome::MinGenome(MinGAFactory *pFactory)
{

	//printf( "%6.4f", dij[0][0] );

	bixelweights.resize(BEAMLETS*ANGLES,1);

	//bixelweights.resize(beamlets, 1);
	for (int i = 0; i < BEAMLETS*ANGLES; i++) {
		bixelweights(i) = 1;
	}

	m_pFactory = pFactory;
}

MinGenome::MinGenome(float bw[BEAMLETS], MinGAFactory *pFactory)
{
	bixelweights.resize(BEAMLETS*ANGLES,1);

	for (int i = 0; i < BEAMLETS*ANGLES; i++) {
		bixelweights(i) = bw[i];
	}

	m_pFactory = pFactory;
}

MinGenome::MinGenome(VectorXf bw, MinGAFactory *pFactory)
{
	bixelweights.resize(BEAMLETS*ANGLES,1);
	bixelweights = Map<VectorXf>(bw.data(), BEAMLETS*ANGLES);

	m_pFactory = pFactory;
}

MinGenome::~MinGenome()
{
}

bool MinGenome::calculateFitness()
{
	Map<MatrixXf> dijMatrix(*dij, DIJ_X, DIJ_Y);
	//std::cout << dijMatrix << std::endl;
	// std::cout << "dijMatrix: " << dijMatrix.size() << std::endl;
	// std::cout << "bixlweights: " << bixelweights.size() << std::endl;

	MatrixXf dose(DIM_X, DIM_Y);

	dose = dijMatrix.transpose() * bixelweights;

	Map<ArrayXf> dose2Array(dose.data(), dose.rows()*dose.cols());
	
	//writeToCSVFile("../problemset/newdosematrix.csv", dose2Array);

	Map<MatrixXi> voi2Matrix(*voiData, DIM_X, DIM_Y);
	Map<ArrayXi> voi2Array(voi2Matrix.data(), DIM_X*DIM_Y);

	Map<ArrayXi> minDoseA(*minDose, DIJ_Y);
	Map<ArrayXi> maxDoseA(*maxDose, DIJ_Y);

	//writeToCSVFile("../problemset/minDoseMatrix.csv", minDoseA.cast<float>());
	//writeToCSVFile("../problemset/voiAreaMatrix.csv", voi2Array.cast<float>());

	//std::cout << minDoseA << std::endl;

	int obj = 0;
  	//std::cout << dose2Array << std::endl;

	for(int row = 0; row < DIJ_Y; ++row)
	{
		if (dose2Array(row) > maxDoseA(row))
		{
			obj += pow(dose2Array(row) - maxDoseA(row), 2) * OD_PEN * 1.0/voiWeights[voi2Array(row)];
		}

		if (minDoseA(row) > dose2Array(row))
		{
			obj += pow(dose2Array(row) - minDoseA(row), 2) * UD_PEN * 1.0/voiWeights[voi2Array(row)];
		} 
	}

	//std::cout << "Obj value: " << obj << std::endl;
	m_fitness = obj;
	
	return true;
}

bool MinGenome::isFitterThan(const mogal::Genome *pGenome) const
{
	const MinGenome *pMinGenome = (const MinGenome *)pGenome;

	if (m_fitness < pMinGenome->m_fitness)
		return true;
	return false;
}

mogal::Genome *MinGenome::reproduce(const mogal::Genome *pGenome) const
{
	double randomNumber = (1.0*((double)rand()/(RAND_MAX + 1.0)));
	const MinGenome *pMinGenome = (const MinGenome *)pGenome;
	MinGenome *pNewGenome = 0;
	
	VectorXf newBWs = VectorXf(bixelweights.size());

	// randomly select some bixelweights

	if (randomNumber < 0.1) {
		for(int bwi = 0; bwi < ANGLES*BEAMLETS; ++bwi)
		{
			double rng = (1.0*((double)rand()/(RAND_MAX + 1.0)));
			if (rng < 0.5) {
				newBWs(bwi) = pMinGenome->bixelweights(bwi);
			} 
		else
		{
			newBWs(bwi) = bixelweights(bwi);
		}
	}
		pNewGenome = new MinGenome(newBWs, m_pFactory);
	}
	else if (randomNumber < 0.98)
		pNewGenome = new MinGenome(bixelweights, m_pFactory);
	else
		pNewGenome = new MinGenome(m_pFactory);
	return pNewGenome;
}

mogal::Genome *MinGenome::clone() const
{
	MinGenome *pNewGenome = new MinGenome(bixelweights, m_pFactory);

	pNewGenome->m_fitness = m_fitness;
	return pNewGenome;
}

void MinGenome::mutate()
{ 
	float randomNumber = (1.0*(rand()/(RAND_MAX + 1.0))); // do mutate or do not mutate (0-1 exclusive)

	//std::cout << smallRandomNumber << std::endl;
	if (randomNumber < 0.05) 
	{
		for(int row = 0; row < ANGLES*BEAMLETS; ++row)
		{
			float smallRandomNumber = (rand() / double(RAND_MAX) - 0.5)*2; // -1 exclusive to +1 exclusive
			smallRandomNumber /= 5;

			bixelweights(row) += smallRandomNumber;
			if (bixelweights(row) <= 0) 
				bixelweights(row) = 0;
		}
		// bixelweights.array() += smallRandomNumber;
	}
	else
	{
		// don't mutate
	}
}

std::string MinGenome::getFitnessDescription() const
{
	char str[256];

	sprintf(str, "%g ", m_fitness);

	return std::string(str);
}

std::string MinGenome::getBixelweightsDescription() const
{
	std::string output = "Bixelweights: \n";

	for (auto b : bixelweights) 
	{
		output += std::to_string(b) + ", ";
	}

	return std::string(output);
}

VectorXf MinGenome::getBixelweights() const
{
	return bixelweights;
}


void MinGenome::writeToCSVFile(string name, MatrixXf matrix)
{
    ofstream file(name.c_str());
    file << matrix.format(CSVFormat);
}