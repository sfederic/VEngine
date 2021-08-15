#pragma once

struct Actor;

struct Component
{
	Actor* owner = nullptr;
	
	virtual void Create() = 0;
};
