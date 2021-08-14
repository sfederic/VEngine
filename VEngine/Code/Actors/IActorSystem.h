#pragma once
#include <vector>

struct Actor;

struct IActorSystem
{
	virtual void GetActors(std::vector<Actor*>& outActors) = 0;
};
