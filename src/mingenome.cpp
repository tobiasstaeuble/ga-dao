#include <mogal/gafactorysingleobjective.h>
#include <mogal/genome.h>
#include <mogal/geneticalgorithm.h>
#include <mogal/randomnumbergenerator.h>
#include <serut/serializationinterface.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <iostream>
#include "mingenome.h"
#include "globals.h"
#include <Eigen/Dense>
#include <fstream>
#include <sys/time.h>
#include <iterator>

using namespace Eigen;
using namespace std;

// Genome implementation

MinGenome::MinGenome(MinGAFactory *pFactory)
{
	m_pFactory = pFactory;
	bixelweights.resize(BEAMLETS*NUM_ANGLES,1);
	for(int i = 0; i < NUM_ANGLES; i++) {
		angles.push_back(Angle(i, 360/NUM_ANGLES*i, pFactory));
	}
	genomeChanged = true;
}

MinGenome::MinGenome(std::vector<Angle> angles, MinGAFactory *pFactory)
{
	m_pFactory = pFactory;
	bixelweights.resize(BEAMLETS*NUM_ANGLES,1);
	this->angles = angles;
	genomeChanged = true;
}

MinGenome::~MinGenome()
{
}

void MinGenome::setActiveFitnessComponent(int i)
{
	m_activeFitness = i;
}

bool MinGenome::calculateFitness()
{
	if (genomeChanged)
	{
		for (int i = 0; i < NUM_ANGLES*BEAMLETS; ++i) 
		{
			bixelweights[i] = 0;
		} 
		
		double tmp = 0;

		for (int i = 0; i < NUM_ANGLES; ++i)
		{
			double totalAngleTime = 0;
			for (int j = 0; j < angles[i].configurations.size(); ++j) 
			{
				totalAngleTime += angles[i].configurations[j].time;
				for (int k = 0; k < BEAMLETS-1; ++k)
				{
					if (angles[i].configurations[j].LL <= k && angles[i].configurations[j].RL > k) {
						bixelweights[i*BEAMLETS + k] += angles[i].configurations[j].time;
					}
				} 
			}
			angles[i].totalTime = totalAngleTime;
			tmp += totalAngleTime;
		}

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
		m_fitness[0] = obj * WEIGHT_DOSE;
		m_fitness[1] = tmp * WEIGHT_TIME;
	}
	
	genomeChanged = false;
	return true;
}

bool MinGenome::isFitterThan(const mogal::Genome *pGenome) const
{
	const MinGenome *pMinGenome = (const MinGenome *)pGenome;

	if (m_fitness[m_activeFitness] < pMinGenome->m_fitness[m_activeFitness])
		return true;
	return false;
}

mogal::Genome *MinGenome::reproduce(const mogal::Genome *pGenome) const
{
	double randomNumber = m_pFactory->getRandomNumberGenerator()->pickRandomNumber();
	const MinGenome *pMinGenome = (const MinGenome *)pGenome;
	MinGenome *pNewGenome = 0;
	
	std::vector<Angle> nAngles;

	for(int i = 0; i < NUM_ANGLES; i++) {
		nAngles.push_back(Angle(i, 360/NUM_ANGLES*i, m_pFactory));
	}

	// crossover
	if (randomNumber < CROSSOVER_RATE) {
		for (int i = 0; i < NUM_ANGLES; ++i)
		{
			for (int j = 0; j < NUM_CONFIGS; ++j) 
			{
				double rng = m_pFactory->getRandomNumberGenerator()->pickRandomNumber();
				// 50-50
				if (rng < 0.5)
				{
					nAngles[i].configurations[j].LL = pMinGenome->angles[i].configurations[j].LL;
					nAngles[i].configurations[j].RL = pMinGenome->angles[i].configurations[j].RL;
					nAngles[i].configurations[j].time = pMinGenome->angles[i].configurations[j].time;
				} else {
					nAngles[i].configurations[j].LL = angles[i].configurations[j].LL;
					nAngles[i].configurations[j].RL = angles[i].configurations[j].RL;
					nAngles[i].configurations[j].time = angles[i].configurations[j].time;
				}
			}
		}
		pNewGenome = new MinGenome(nAngles, m_pFactory);
	}

	// reset
	else if (randomNumber > 1-RESET_RATE) 
	{
		pNewGenome = new MinGenome(m_pFactory);
	}
		
	// change nothing / no breeding
	else 
	{
		pNewGenome = new MinGenome(angles, m_pFactory);
	}
		
	return pNewGenome;
}

mogal::Genome *MinGenome::clone() const
{
	MinGenome *pNewGenome = new MinGenome(angles, m_pFactory);

	pNewGenome->m_fitness[0] = m_fitness[0];
	pNewGenome->m_fitness[1] = m_fitness[1];
	return pNewGenome;
}

void MinGenome::mutate()
{ 
	double randomNumber = m_pFactory->getRandomNumberGenerator()->pickRandomNumber(); // do mutate or do not mutate (0-1 exclusive)

	// mutation
	if (randomNumber < MUTATION_RATE)
	{
		for (int i = 0; i < angles.size(); ++i)
		{
			for (int j = 0; j < angles[i].configurations.size(); j++)
			{
				double sRandomNumber = (m_pFactory->getRandomNumberGenerator()->pickRandomNumber() - 0.5)*2; // -1 exclusive to +1 exclusive
				
				if (sRandomNumber < 1) {
					angles[i].configurations[j].shuffleConfiguration();
				}
			}
		}
	}
	genomeChanged = true;
}

std::string MinGenome::getFitnessDescription() const
{
	char str[256];

	sprintf(str, "Fitness : %g %g ", m_fitness[0], m_fitness[1] );
	return std::string(str);
}

std::string MinGenome::getFitness(int i) const
{
	return std::to_string(m_fitness[i]);
}

double MinGenome::getFitnessF(int i) const
{
	return m_fitness[i];
}

std::string MinGenome::getBixelweightsDescription() const
{
	std::string output = "Bixelweights: \n";

	for (double b : bixelweights) 
	{
		output += std::to_string(b) + ", ";
	}

	return std::string(output);
}

std::string MinGenome::getTimingDescription() const
{
	std::string output = "Total collimator time per angle: \n";

	std::vector<double> timings;
	for (Angle a : angles) 
	{
		output += std::to_string(a.totalTime) + ", ";
	}

	return std::string(output);
}


VectorXf MinGenome::getBixelweights() const
{
	return bixelweights;
}

std::vector<double> MinGenome::serializeAngles() const
{
	std::vector<double> sAngles(m_pFactory->getMaximalGenomeSize()/sizeof(double));

	for (int i = 0; i < NUM_ANGLES; i++) 
	{
		for (int j = 0; j < NUM_CONFIGS; j++)
		{
			sAngles[i*NUM_CONFIGS*3+j*3] = angles[i].configurations[j].LL;
			sAngles[i*NUM_CONFIGS*3+j*3+1] = angles[i].configurations[j].RL;
			sAngles[i*NUM_CONFIGS*3+j*3+2] = angles[i].configurations[j].time;
		}
	}
	//for (auto i: sAngles)
 	//	std::cout << i << ' ';
 	//struct timeval tp;
	//gettimeofday(&tp, NULL);
	//long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
 	//writeVectorToCSVFile("../output/blub-" + std::to_string(ms), sAngles);
	return sAngles;
}

void MinGenome::writeToCSVFile(string name, MatrixXf matrix)
{
    ofstream file(name.c_str());
    file << matrix.format(CSVFormat);
}

void MinGenome::writeVectorToCSVFile(string name, std::vector<double> vec) const
{
	std::ofstream FILE(name, std::ios::out | std::ofstream::binary);
    std::ostream_iterator<double> osi{FILE," "};
    std::copy(vec.begin(), vec.end(), osi);
}