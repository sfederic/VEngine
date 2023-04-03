#include "vpch.h"
#include "UID.h"
#include <random>
#include <unordered_set>
#include <limits>

std::random_device randomDevice;
std::mt19937 randomGenerator(randomDevice());
std::uniform_int_distribution<UID> uidDist(std::numeric_limits<UID>::min(), std::numeric_limits<UID>::max());

std::unordered_set<UID> everyUID;

void ResetUIDCache()
{
	everyUID.clear();
}

UID GenerateUID()
{
	UID value = uidDist(randomGenerator);
	assert(everyUID.find(value) == everyUID.end());
	everyUID.emplace(value);
	return value;
}
