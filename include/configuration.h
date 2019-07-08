#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <Eigen/Dense>
#include "constants.h"
#include "angle.h"
#include "mingafactory.h"

using namespace Eigen;
using namespace std;

class Angle;
class MinGAFactory;

class Configuration
{
public:
	Configuration(int LL, int RL);
	Configuration();

	void updateConfiguration(int LL, int RL, double time);
	void updateConfiguration(int LL, int RL, double time, double fitness);
	void shuffleConfiguration();

	int LL, RL;
	double time;
	double fitness;
	bool changed;
	Angle *parent;
	MinGAFactory *gaFactory;

private:

};

#endif