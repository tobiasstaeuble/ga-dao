#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Eigen/Dense>
#include "constants.h"

using namespace Eigen;
using namespace std;

class Configuration
{
public:
	Configuration(int LL, int RL);
	Configuration();

	void updateConfiguration(int LL, int RL, float time);
	void updateConfiguration(int LL, int RL, float time, float fitness);
	void shuffleConfiguration();

	int LL, RL;
	float time;
	float fitness;
	bool changed;

private:

};

#endif