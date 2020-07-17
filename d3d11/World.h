#pragma once

#include <vector>

//TODO: need a GetWorld() equivalent on the ending backend, too muc to always be copying pointers to new functions

class World
{
public:

	std::vector<class ActorSystem*> actorSystems;
	char name[64];
};
