#pragma once
#include <random>
#include "Vector2.h"

class Random
{
public:
	static void init();

	//Seed the generaor with the specified int
	// NOTE : you should generaaly not need to manually use this
	static void seed(unsigned int seed);
	
	//get a float between 0.0f and 1.0f
	static float getFloat();

	//get a float from the specified range
	static float getFloatRange(float min, float max);
	
	//get a int from the specified range
	static int getIntRange(int min, int max);

	//get a random vector given the min/max bounds
	static Vector2 getVector(const Vector2& min, const Vector2& max);

private:
	static std::mt19937 sGenerator;
};

