#include "angle.h"
#include "constants.h"

Angle::Angle() {

}

Angle::Angle(int id, float deg) {
	this->id = id;
	this->deg = deg;

	configurations.resize(10);
	
	for (int i = 0; i < configurations.size(); ++i)
	{
		configurations[i].LL = 0;
		configurations[i].RL = BEAMLETS-1;
	}
}

Angle::Angle(int id) {
	this->id = id;
}