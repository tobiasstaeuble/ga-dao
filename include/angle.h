#ifndef ANGLE_H
#define ANGLE_H

#include <Eigen/Dense>
#include "constants.h"
#include "configuration.h"
#include <vector>

using namespace Eigen;
using namespace std;

class Angle
{
public:
	Angle();
	Angle(int id, float deg);
	Angle(int id);
	
	int id;
	float deg;
	std::vector<Configuration> configurations;

private:

};

#endif