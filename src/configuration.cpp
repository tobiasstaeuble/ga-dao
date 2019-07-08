#include "configuration.h"

Configuration::Configuration(int LL, int RL) {
	this->LL = LL;
	this->RL = RL;
	changed = true;
}

Configuration::Configuration() {

}

void Configuration::updateConfiguration(int L, int R, double time) {
	this->LL = LL;
	this->RL = RL;
	this->time = time;
	changed = true;
}

void Configuration::updateConfiguration(int L, int R, double time, double fitness) {
	this->LL = LL;
	this->RL = RL;
	this->time = time;
	this->fitness = fitness;
	changed = true;
}

void Configuration::shuffleConfiguration() {

	this->LL = (this->gaFactory->getRandomNumberGenerator()->pickRandomNumber())*BEAMLETS;
	this->LL = (this->LL > BEAMLETS) ? BEAMLETS : this->LL;

	this->RL = LL + (this->gaFactory->getRandomNumberGenerator()->pickRandomNumber())*(BEAMLETS-LL);
	this->RL = (this->RL > BEAMLETS) ? BEAMLETS : this->RL;

	this->time += (this->gaFactory->getRandomNumberGenerator()->pickRandomNumber() - 0.5)*2;
	this->time = (this->time < 0) ? 0 : this->time;
	this->time = (this->time > 1000) ? 0 : this->time; 


	this->fitness = 0;
	changed = true;
}