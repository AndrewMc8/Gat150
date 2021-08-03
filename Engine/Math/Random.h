#pragma once


namespace nc
{
	void SeedRandom(unsigned int seed);

	float Random();
	float RandomRange(float min, float max);

	int RandomInt();
	int RandomRangeInt(int min, int max);
}