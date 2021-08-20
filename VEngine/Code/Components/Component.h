#pragma once
#include "Properties.h"

struct Actor;

struct Component
{
	Actor* owner = nullptr;
	
	int index = -1;

	virtual void Create() = 0;
	virtual void Destroy() = 0;
	virtual Properties GetProperties() = 0;
};
