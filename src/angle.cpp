#include "angle.h"
#include "constants.h"
#include <iostream>

Angle::Angle() {
	configurations.resize(NUM_CONFIGS);
}

Angle::Angle(int id, float deg) {
	this->id = id;
	this->deg = deg;

	configurations.resize(NUM_CONFIGS);

	for (int i = 0; i < configurations.size(); ++i)
	{
		configurations[i].LL = 0;
		configurations[i].RL = BEAMLETS-1;
		configurations[i].time = 0;
	}
}

Angle::Angle(int id) {
	this->id = id;
	configurations.resize(NUM_CONFIGS);
}