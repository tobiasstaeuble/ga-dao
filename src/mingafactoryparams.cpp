#include "mingafactoryparams.h"

// Factory parameters implementation

MinGAFactoryParams::MinGAFactoryParams()
{
}

MinGAFactoryParams::~MinGAFactoryParams()
{
}

bool MinGAFactoryParams::write(serut::SerializationInterface &si) const
{
	setErrorString("Serialization is not implemented yet");
	return false;
}

bool MinGAFactoryParams::read(serut::SerializationInterface &si)
{
	setErrorString("Serialization is not implemented yet");
	return false;
}