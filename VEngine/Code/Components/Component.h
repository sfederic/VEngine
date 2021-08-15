#pragma once
#include <vector>
#include "Actors/Actor.h"

struct Component
{
	Actor* owner = nullptr;
	Component* parent = nullptr;
	std::vector<Component*> children;

	virtual void Create() = 0;
};
