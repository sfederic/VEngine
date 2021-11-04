#include "UID.h"
#include <random>
#include <limits>

std::random_device randomDevice;
std::mt19937 randomGenerator(randomDevice());
std::uniform_int_distribution<uid> uidDist(std::numeric_limits<uid>::min(), std::numeric_limits<uid>::max());

uid GenerateUID()
{
	uid value = uidDist(randomGenerator);
	return uid();
}
