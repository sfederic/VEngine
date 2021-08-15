#pragma once
#include <vector>

struct Actor;

struct Component
{
	Actor* owner;
	Component* parent;
	std::vector<Component*> children;

	virtual void Create() = 0;
};
