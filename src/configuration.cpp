#include "configuration.h"

Configuration::Configuration(int LL, int RL) {
	this->LL = LL;
	this->RL = RL;
	changed = true;
}

Configuration::Configuration() {

}

void Configuration::updateConfiguration(int L, int R, float time) {
	this->LL = LL;
	this->RL = RL;
	this->time = time;
	changed = true;
}

void Configuration::updateConfiguration(int L, int R, float time, float fitness) {
	this->LL = LL;
	this->RL = RL;
	this->time = time;
	this->fitness = fitness;
	changed = true;
}

void Configuration::shuffleConfiguration() {
	this->LL = (1.0*(rand()/(RAND_MAX + 1.0)))*BEAMLETS;
	this->RL = LL + 1;
	this->time += (rand() / double(RAND_MAX) - 0.5)*2;
	this->time = (this->time < 0) ? 0 : this->time;
	this->fitness = 0;
	changed = true;
}