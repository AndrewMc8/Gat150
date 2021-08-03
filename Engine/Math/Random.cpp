#include "Random.h"
#include <stdlib.h>

namespace nc
{
	void SeedRandom(unsigned int seed)
	{
		srand(seed);
	}

	float Random()
	{
		return static_cast<float>(rand()) / RAND_MAX;
	}

	float RandomRange(float min, float max)
	{
		return min + (max - min) * Random();
	}

	int RandomInt()
	{
		return rand();
	}

	int RandomRangeInt(int min, int max)
	{
		return min + rand() % (min - max);
	}
}