#include "vpch.h"
#include "UID.h"
#include <random>
#include <limits>

std::random_device randomDevice;
std::mt19937 randomGenerator(randomDevice());
std::uniform_int_distribution<UID> uidDist(std::numeric_limits<UID>::min(), std::numeric_limits<UID>::max());

UID GenerateUID()
{
	UID value = uidDist(randomGenerator);
	return value;
}
