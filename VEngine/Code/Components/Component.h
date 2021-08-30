#pragma once
#include "Properties.h"

struct Actor;

struct Component
{
	Actor* owner = nullptr;
	std::string name;
	int index = -1;

	virtual void Tick(double deltaTime) = 0;
	virtual void Create() = 0;
	virtual void Destroy() = 0;
	virtual Properties GetProps() = 0;
};
