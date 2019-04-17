#ifndef SIMPLERNDGEN_H
#define SIMPLERNDGEN_H 

class SimpleRndGen : public mogal::RandomNumberGenerator
{
public:
	SimpleRndGen() { }
	~SimpleRndGen() { }
	
	double pickRandomNumber() const
	{
		double randomNumber = (1.0*((double)rand()/(RAND_MAX + 1.0)));

		return randomNumber;
	}
};

#endif