#include "ActorSystemFactory.h"
#include <unordered_map>

uint64_t actorSystemID = 0;
std::unordered_map<size_t, ActorSystem*> actorSystemMap;

void ActorSystemFactory::Register(ActorSystem* actorSystem)
{
	actorSystemMap.insert(std::pair(actorSystemID, actorSystem));
	actorSystemID++;
}
