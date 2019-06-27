#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Eigen/Dense>
#include "constants.h"
#include "angle.h"

using namespace Eigen;
using namespace std;

class Angle;

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
	Angle *parent;

private:

};

#endif