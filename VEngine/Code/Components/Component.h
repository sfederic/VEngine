#pragma once
#include "Properties.h"

struct Actor;

struct Component
{
	Actor* owner = nullptr;
	
	virtual void Create() = 0;
	virtual Properties GetProperties() = 0;
};
