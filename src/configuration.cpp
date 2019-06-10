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