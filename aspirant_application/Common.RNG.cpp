#include "Common.RNG.h"
#include <stdlib.h>
#include <time.h>
namespace common::RNG
{
	void Seed()
	{
		srand((unsigned int)time(nullptr));
	}

	int FromRange(int minimum, int maximum)
	{
		return rand() % (maximum - minimum) + minimum;
	}

}