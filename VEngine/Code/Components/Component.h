#pragma once
#include "Properties.h"
#include <memory>

struct Actor;

struct Component
{
	//TODO: there's something wrong with owner here. It always ends up crashing with junk data.
	Actor* owner = nullptr; 

	std::string name;
	int index = -1;

	virtual void Tick(double deltaTime) = 0;
	virtual void Create() = 0;
	virtual void Destroy() = 0;
	virtual Properties GetProps() = 0;
};
