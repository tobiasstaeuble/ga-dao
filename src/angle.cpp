#include "angle.h"
#include "constants.h"
#include <iostream>

Angle::Angle(MinGAFactory *factory) {
	this->gaFactory = factory;
	configurations.resize(NUM_CONFIGS);

	for (int i = 0; i < configurations.size(); ++i)
	{
		configurations[i].gaFactory = factory;
		configurations[i].parent = (Angle *)this;
		configurations[i].shuffleConfiguration();
	}
}

Angle::Angle(int id, double deg, MinGAFactory *factory) {
	this->id = id;
	this->gaFactory = factory;
	this->deg = deg;

	configurations.resize(NUM_CONFIGS);

	for (int i = 0; i < configurations.size(); ++i)
	{
		configurations[i].gaFactory = factory;
		configurations[i].parent = (Angle *)this;
		configurations[i].shuffleConfiguration();
	}
}

Angle::Angle(int id, MinGAFactory *factory) {
	this->id = id;
	this->gaFactory = factory;
	configurations.resize(NUM_CONFIGS);

	for (int i = 0; i < configurations.size(); ++i)
	{
		configurations[i].gaFactory = factory;
		configurations[i].parent = (Angle *)this;
		configurations[i].shuffleConfiguration();

	}
}