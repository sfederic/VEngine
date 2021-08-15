#pragma once
#include <typeindex>

struct IActorSystem;

namespace ActorSystemCache
{
	void Add(std::type_index type, IActorSystem* actorSystem);
};
