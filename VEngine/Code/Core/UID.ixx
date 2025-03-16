export module Core.UID;

import Core.VAssert;
import <random>;
import <unordered_set>;
import <limits>;

//Unique Identifier
export typedef unsigned int UID;

std::random_device randomDevice;
std::mt19937 randomGenerator(randomDevice());
std::uniform_int_distribution<UID> uidDist(std::numeric_limits<UID>::min(), std::numeric_limits<UID>::max());

std::unordered_set<UID> everyUID;

export void ResetUIDCache()
{
	everyUID.clear();
}

export UID GenerateUID()
{
	UID value = uidDist(randomGenerator);
	VAssert(everyUID.find(value) == everyUID.end(), "UID already exists.");
	everyUID.emplace(value);
	return value;
}
