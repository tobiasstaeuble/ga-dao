#ifndef ANGLE_H
#define ANGLE_H

#include <Eigen/Dense>
#include "constants.h"
#include "configuration.h"
#include "mingafactory.h"
#include <vector>

using namespace Eigen;
using namespace std;

class Configuration;
class MinGAFactory;

class Angle
{
public:
	Angle(MinGAFactory *factory);
	Angle(int id, double deg, MinGAFactory *factory);
	Angle(int id, MinGAFactory *factory);
	
	int id;
	double deg;
	std::vector<Configuration> configurations;
	double totalTime;
	

private:
	MinGAFactory *gaFactory;
	friend class Configuration;
	friend class MinGAFactory;
};

#endif