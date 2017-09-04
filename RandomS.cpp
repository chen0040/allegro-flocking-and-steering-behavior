#include "RandomS.h"
#include <cstdlib>
#include <cmath>
#include <ctime>

RandomS::RandomS()
{
	srand(time(NULL));
}

RandomS* RandomS::GetInstance()
{
	static RandomS theInstance;
	return &theInstance;
}

double RandomS::nextDouble() const
{
	return static_cast<double>(rand()) / (static_cast<double>(RAND_MAX) + 1.0);
}

double RandomS::nextDouble(double lower_bound, double upper_bound) const
{
	return lower_bound + nextDouble() * (upper_bound - lower_bound);
}

int RandomS::nextInt(int upper_bound) const
{
	return rand() % upper_bound;
}