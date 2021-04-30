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

	size_t FromRange(size_t minimum, size_t maximum)
	{
		return (size_t)FromRange((int)minimum, (int)maximum);
	}
}