#include "configuration.h"

Configuration::Configuration(int LL, int RL) {
	this->LL = LL;
	this->RL = RL;
    this->leafTime = (1.0*((double)rand()/(RAND_MAX + 1.0)));
    
	changed = true;
}

Configuration::Configuration() {
    this->leafTime = (1.0*((double)rand()/(RAND_MAX + 1.0)));
	changed = true;
}

void Configuration::updateConfiguration(int L, int R, double leafTime) {
	this->LL = LL;
	this->RL = RL;
	this->leafTime = leafTime;
	changed = true;
}

void Configuration::updateConfiguration(int L, int R, double leafTime, double fitness) {
	this->LL = LL;
	this->RL = RL;
	this->leafTime = leafTime;
	this->fitness = fitness;
	changed = true;
}

void Configuration::shuffleConfiguration() {

	this->LL = (this->gaFactory->getRandomNumberGenerator()->pickRandomNumber())*BEAMLETS;
	this->LL = (this->LL > BEAMLETS) ? BEAMLETS : this->LL;

	this->RL = LL + (this->gaFactory->getRandomNumberGenerator()->pickRandomNumber())*(BEAMLETS-LL);
	this->RL = (this->RL > BEAMLETS) ? BEAMLETS : this->RL;

	leafTime += (this->gaFactory->getRandomNumberGenerator()->pickRandomNumber() - 0.5)*2;
	leafTime = (leafTime < 0) ? 0 : leafTime;

	this->fitness = 0;
	changed = true;
}